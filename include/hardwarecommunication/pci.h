#ifndef __BATOS__HARDWARECOMMUNICATION__PCI_H
#define __BATOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

namespace batos {
    namespace hardwarecommunication {

        enum BaseAddressRagisterType {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister {
            public:
                bool prefetchable;
                batos::common::uint8_t* address;
                batos::common::uint32_t* size;
                BaseAddressRagisterType type;
        };
    
        class PeripheralComponentInterconnectDeviceDescriptor {
            public:
                batos::common::uint32_t portBase;
                batos::common::uint32_t interrupt;

                batos::common::uint16_t bus;
                batos::common::uint16_t device;
                batos::common::uint16_t function;

                batos::common::uint16_t vendor_id;
                batos::common::uint16_t device_id;

                batos::common::uint8_t class_id;
                batos::common::uint8_t subClass_id;
                batos::common::uint8_t interface_id;

                batos::common::uint8_t revision;

                PeripheralComponentInterconnectDeviceDescriptor();
                ~PeripheralComponentInterconnectDeviceDescriptor();

        };

        class PeripheralComponentInterconnectController {

            Port32Bit dataPort;
            Port32Bit commandPort;
        
        
            public:
                PeripheralComponentInterconnectController();
                ~PeripheralComponentInterconnectController();

                batos::common::uint16_t Read(
                    batos::common::uint16_t bus,
                    batos::common::uint16_t device,
                    batos::common::uint16_t function,
                    batos::common::uint32_t registeroffset);
                    
                void Write(batos::common::uint16_t bus,
                    batos::common::uint16_t device,
                    batos::common::uint16_t function,
                    batos::common::uint32_t registeroffset,
                    batos::common::uint16_t value);

                bool DeviceHasFunctions(batos::common::uint16_t bus,
                    batos::common::uint16_t device);
                
                void SelectDrivers(batos::drivers::DriverManager* driverManager, batos::hardwarecommunication::InterruptManager* interrupts);
                batos::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, batos::hardwarecommunication::InterruptManager* interrupts);
                PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(
                    batos::common::uint16_t bus,
                    batos::common::uint16_t device,
                    batos::common::uint16_t function
                );
                BaseAddressRegister GetBaseAddressResister(
                    batos::common::uint16_t bus,
                    batos::common::uint16_t device,
                    batos::common::uint16_t function,
                    batos::common::uint16_t bar);
            
        };
    }
}

#endif