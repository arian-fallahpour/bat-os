#ifndef __BATOS__GUI__DESKTOP_H
#define __BATOS__GUI__DESKTOP_H

#include <drivers/mouse.h>
#include <gui/widget.h>

namespace batos {
    namespace gui {
        class Desktop :
            public batos::gui::CompositeWidget,
            public batos::drivers::MouseEventHandler {
            protected:
                batos::common::uint32_t MouseX, MouseY;

            public:
                Desktop(
                    batos::common::int32_t w,
                    batos::common::int32_t h,
                    batos::common::uint8_t r,
                    batos::common::uint8_t g,
                    batos::common::uint8_t b
                );
                ~Desktop();

                void Draw(batos::common::GraphicsContext* gc);
                void OnMouseDown(batos::common::uint8_t button);
                void OnMouseUp(batos::common::uint8_t button);
                void
                OnMouseMove(batos::common::int8_t x, batos::common::int8_t y);
        };
    }  // namespace gui
}  // namespace batos

#endif
