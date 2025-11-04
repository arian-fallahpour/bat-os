#include <hardwarecommunication/interrupts.h>

using namespace batos;
using namespace batos::common;
using namespace batos::hardwarecommunication;
using namespace batos::multitasking;

void printf(char* str);
void printfHex(uint8_t key);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager) {
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if (interruptManager->handlers[interruptNumber] == this) {
        interruptManager->handlers[interruptNumber] = 0;
    }
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;

void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t DescriptorPrivilegeLevel,
    uint8_t DescriptorType
) {
    const uint8_t IDT_DESC_PRESENT = 0x80;      

    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(        
    uint16_t hardwareInterruptOffset,
    GlobalDescriptorTable* gdt,
    TaskManager* taskManager
): 
    picMasterCommand(0x20),
    picMasterData(0x21),
    picSlaveCommand(0xA0),
    picSlaveData(0xA1){

    this->taskManager = taskManager;
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint16_t CodeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRIPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; i++) {
        handlers[i] = 0;
        SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRIPT_GATE); // Set to ignore all by default
    }

    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRIPT_GATE); // wired from HandleInterruptRequest0x00 in ASM (0x20 + 0x00 = 0x20)
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRIPT_GATE); // wired from HandleInterruptRequest0x01 in ASM (0x20 + 0x01 = 0x21)
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRIPT_GATE); // wired from HandleInterruptRequest0x01 in ASM (0x20 + 0x01 = 0x21)

    picMasterCommand.Write(0x11); // Initialize PIC
    picSlaveCommand.Write(0x11);

    picMasterData.Write(hardwareInterruptOffset); // Remap PIC
    picSlaveData.Write(hardwareInterruptOffset + 8);

    picMasterData.Write(0x04); // Setup cascading
    picSlaveData.Write(0x02);

    picMasterData.Write(0x01); // Set 8086/88 (MCS-80/85) mode
    picSlaveData.Write(0x01);

    picMasterData.Write(0x00); // Unmask all interrupts
    picSlaveData.Write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));    
}

InterruptManager::~InterruptManager() {
    Deactivate();
}

uint16_t InterruptManager::HardwareInterruptOffset() {
    return hardwareInterruptOffset;
}

void InterruptManager::Activate() {
    if (ActiveInterruptManager != 0){
        ActiveInterruptManager->Deactivate();
    }

    ActiveInterruptManager = this;

    asm("sti"); 
}

void InterruptManager::Deactivate() {
    if (ActiveInterruptManager == this){
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if (ActiveInterruptManager != 0) {
        return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp) {
    if (handlers[interruptNumber] != 0) {
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    } else if (interruptNumber != hardwareInterruptOffset) {
        printf("UNHANDLED INTERRUPT");
        printfHex(interruptNumber);
    }

    if (interruptNumber == hardwareInterruptOffset) {
        esp = (uint32_t)taskManager->Schedule((CPUState*)esp);
    }

    if (hardwareInterruptOffset <= interruptNumber && interruptNumber < hardwareInterruptOffset + 16) {
        picMasterCommand.Write(0x20);
        if (hardwareInterruptOffset + 8 <= interruptNumber) {
            picSlaveCommand.Write(0x20);  // Send EOI to slave PIC
        }
    }
    
    return esp;
}

