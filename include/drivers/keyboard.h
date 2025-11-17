#ifndef __BATOS__DRIVERS__KEYBOARD_H
#define __BATOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

namespace batos {
    namespace drivers {
        class KeyboardEventHandler {
            public:
                KeyboardEventHandler();

                virtual void OnKeyDown(char c);
                virtual void OnKeyUp(char c);
        };

        class KeyboardDriver :
            public batos::hardwarecommunication::InterruptHandler,
            public batos::drivers::Driver {
                batos::hardwarecommunication::Port8Bit dataport;
                batos::hardwarecommunication::Port8Bit commandport;

                batos::drivers::KeyboardEventHandler* handler;

            public:
                KeyboardDriver(
                    batos::hardwarecommunication::InterruptManager* manager,
                    batos::drivers::KeyboardEventHandler* handler
                );
                ~KeyboardDriver();

                virtual batos::common::uint32_t HandleInterrupt(
                    batos::common::uint32_t esp
                );
                virtual void Activate();
        };
    }  // namespace drivers
};  // namespace batos

#endif