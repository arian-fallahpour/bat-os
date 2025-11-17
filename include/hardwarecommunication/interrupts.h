#ifndef __BATOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __BATOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/port.h>
#include <multitasking.h>

namespace batos {
    namespace hardwarecommunication {
        class InterruptManager;

        class InterruptHandler {
            protected:
                batos::common::uint8_t interruptNumber;
                batos::hardwarecommunication::InterruptManager*
                    interruptManager;

                InterruptHandler(
                    batos::common::uint8_t interruptNumber,
                    batos::hardwarecommunication::InterruptManager*
                        interruptManager
                );
                ~InterruptHandler();

            public:
                virtual batos::common::uint32_t HandleInterrupt(
                    batos::common::uint32_t esp
                );
        };

        class InterruptManager {
                friend class batos::hardwarecommunication::InterruptHandler;

            protected:
                static InterruptManager* ActiveInterruptManager;
                batos::hardwarecommunication::InterruptHandler* handlers[256];
                batos::common::uint16_t hardwareInterruptOffset;
                batos::multitasking::TaskManager* taskManager;

                struct GateDescriptor {
                        batos::common::uint16_t handlerAddressLowBits;
                        batos::common::uint16_t gdt_codeSegmentSelector;
                        batos::common::uint8_t reserved;
                        batos::common::uint8_t access;
                        batos::common::uint16_t handlerAddressHighBits;
                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer {
                        batos::common::uint16_t size;
                        batos::common::uint32_t base;
                } __attribute__((packed));

                static void SetInterruptDescriptorTableEntry(
                    batos::common::uint8_t interruptNumber,
                    batos::common::uint16_t codeSegmentSelectorOffset,
                    void (*handler)(),
                    batos::common::uint8_t DescriptorPrivilegeLevel,
                    batos::common::uint8_t DescriptorType
                );

                batos::hardwarecommunication::Port8BitSlow picMasterCommand;
                batos::hardwarecommunication::Port8BitSlow picMasterData;
                batos::hardwarecommunication::Port8BitSlow picSlaveCommand;
                batos::hardwarecommunication::Port8BitSlow picSlaveData;

            public:
                InterruptManager(
                    batos::common::uint16_t hardwareInterruptOffset,
                    GlobalDescriptorTable* gdt,
                    batos::multitasking::TaskManager* taskManager
                );
                ~InterruptManager();

                batos::common::uint16_t HardwareInterruptOffset();

                void Activate();
                void Deactivate();

                static batos::common::uint32_t handleInterrupt(
                    batos::common::uint8_t interruptNumber,
                    batos::common::uint32_t esp
                );
                batos::common::uint32_t DoHandleInterrupt(
                    batos::common::uint8_t interruptNumber,
                    batos::common::uint32_t esp
                );

                static void IgnoreInterruptRequest();
                static void HandleInterruptRequest0x00();
                static void HandleInterruptRequest0x01();
                static void HandleInterruptRequest0x0C();
        };
    }  // namespace hardwarecommunication
};  // namespace batos

#endif