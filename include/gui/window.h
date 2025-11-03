#ifndef __BATOS__GUI__WINDOW_H
#define __BATOS__GUI__WINDOW_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace batos {
    namespace gui {
        class Window : public batos::gui::CompositeWidget, public batos::drivers::MouseEventHandler {
            protected:
                bool Dragging;
            
            public:
                Window(
                    Widget* parent, 
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::int32_t w,
                    batos::common::int32_t h,
                    batos::common::int8_t r,
                    batos::common::int8_t g,
                    batos::common::int8_t b
                );
                ~Window();

                void OnMouseDown(batos::common::int32_t x, batos::common::int32_t y, batos::common::uint8_t button);
                void OnMouseUp(batos::common::int32_t x, batos::common::int32_t y, batos::common::uint8_t button);
                void OnMouseMove(batos::common::int32_t oldx, batos::common::int32_t oldy, batos::common::int32_t newx, batos::common::int32_t newy);

            
        };
    }
}

#endif