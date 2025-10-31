#ifndef __BATOS__DRIVERS__MOUSE_H
#define __BATOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace batos {
    namespace drivers {
        class MouseEventHandler {
            public:
                MouseEventHandler();

                virtual void OnActivate();
                virtual void OnMouseDown(batos::common::uint8_t button);
                virtual void OnMouseUp(batos::common::uint8_t button);
                virtual void OnMouseMove(batos::common::int8_t x, batos::common::int8_t y);
        };
        
        class MouseDriver: public batos::hardwarecommunication::InterruptHandler, public batos::drivers::Driver {
            batos::hardwarecommunication::Port8Bit dataport;
            batos::hardwarecommunication::Port8Bit commandport;

            batos::common::uint8_t buffer[3];
            batos::common::uint8_t offset;
            batos::common::uint8_t buttons;

            MouseEventHandler* handler;

            batos::common::int8_t x, y;

            public:
                MouseDriver(batos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
                ~MouseDriver();

                virtual batos::common::uint32_t HandleInterrupt(batos::common::uint32_t esp);
                virtual void Activate();
        };
    }
};





#endif