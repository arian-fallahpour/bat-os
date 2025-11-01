#include <hardwarecommunication/pci.h>

using namespace batos::common;
using namespace batos::drivers;
using namespace batos::hardwarecommunication;

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor() {}
PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor() {}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController() 
:   dataPort(0xCFC),
    commandPort(0xCF8) {

}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController() {}

batos::common::uint16_t PeripheralComponentInterconnectController::Read(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint32_t registeroffset) {
    uint32_t id = 
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.Write(id);
    uint16_t result = dataPort.Read();
    return result >> (8* (registeroffset % 4));
}

void PeripheralComponentInterconnectController::Write(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint32_t registeroffset,
    batos::common::uint16_t value) {
        uint32_t id = 
            0x1 << 31
            | ((bus & 0xFF) << 16)
            | ((device & 0x1F) << 11)
            | ((function & 0x07) << 8)
            | (registeroffset & 0xFC);
        commandPort.Write(id);
        dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(
    batos::common::uint16_t bus,
    batos::common::uint16_t device) {
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void printf(char* str);
void printfHex(uint8_t);

void PeripheralComponentInterconnectController::SelectDrivers(batos::drivers::DriverManager* driverManager, batos::hardwarecommunication::InterruptManager* interrupts ) {
    for (int bus = 0; bus < 8; bus++) {
        for (int device = 0; device < 32; device++) {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for (int function = 0; function < numFunctions; function++) {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
                    continue;
                }

                for (int barNum = 0; barNum < 6; barNum++) {
                    BaseAddressRegister bar = GetBaseAddressResister(bus, device, function, barNum);
                    if (bar.address && (bar.type == InputOutput)) {
                        dev.portBase = (uint32_t)bar.address;
                    }

                    Driver* driver = GetDriver(dev, interrupts);
                    if (driver != 0) {
                        driverManager->addDriver(driver);
                    }
                }

                // NOTE: These values don't show the same ones as in the video, so if a bug were to occur, this could be a reason
                // Demonstration purposes only
                printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(function & 0xFF);
                
                printf(", VENDOR ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                
                printf(", DEVICE ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressResister(
    batos::common::uint16_t bus,
    batos::common::uint16_t device,
    batos::common::uint16_t function,
    batos::common::uint16_t bar
) {
    BaseAddressRegister result;

    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4 * headertype);
    if (bar >= maxBARs) {
        return result;
    }

    uint32_t bar_value = Read(bus, device, function, 0x10 + 4 * bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;

    if (result.type == MemoryMapping) {
        switch ((bar_value >> 1) & 0x3) {
            case 0: // 32 bit BAR
            case 1: // 20 bit BAR
            case 2: // 64 bit VAR
                break;
            // Go to lowlevel.ey for more
        }
    } 

    // Input output
    else {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
};

Driver* PeripheralComponentInterconnectController::GetDriver(
    PeripheralComponentInterconnectDeviceDescriptor dev, 
    batos::hardwarecommunication::InterruptManager* interrupts
) {

    // We are hard coding drivers into kernel because we don't have access to harddrive yet
    switch (dev.vendor_id) {
        case 0x1022: // AMD
            switch (dev.device_id) {
                case 0xc2000: // ma79c973
                    printf("AMD amd79c973\n");
                    break;
                    // return new AMDDriver(...); 
                }
                break;
        case 0x8086: // Intel
        break;
    }
    
    switch (dev.class_id) {
        case 0x03: // Graphics
            switch(dev.subClass_id) {
                case 0x00: // VGA
                printf("VGA\n");
                    break;
            }
            break;
    }

    return 0;
};

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(
    batos::common::uint16_t bus,
    batos::common::uint16_t device,
    batos::common::uint16_t function
) {
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;
    
    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subClass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3c);

    return result;
}