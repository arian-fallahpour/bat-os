#ifndef __BATOS__GUI__WIDGET_H
#define __BATOS__GUI__WIDGET_H

#include <common/graphicscontext.h>
#include <common/types.h>
#include <drivers/keyboard.h>

namespace batos {
    namespace gui {
        class Widget : public batos::drivers::KeyboardEventHandler {
            protected:
                Widget* parent;
                batos::common::int32_t x, y;
                batos::common::int32_t w, h;

                batos::common::int8_t r, g, b;
                bool isFocusable;

            public:
                Widget(
                    Widget* parent,
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::int32_t w,
                    batos::common::int32_t h,
                    batos::common::int8_t r,
                    batos::common::int8_t g,
                    batos::common::int8_t b
                );
                ~Widget();

                virtual void GetFocus(Widget* widget);
                virtual void MovelToScreen(
                    batos::common::int32_t& x,
                    batos::common::int32_t& y
                );
                virtual bool ContainsCoordinate(
                    batos::common::int32_t x,
                    batos::common::int32_t y
                );

                virtual void Draw(batos::common::GraphicsContext* gc);
                virtual void OnMouseDown(
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::uint8_t button
                );
                virtual void OnMouseUp(
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::uint8_t button
                );
                virtual void OnMouseMove(
                    batos::common::int32_t oldx,
                    batos::common::int32_t oldy,
                    batos::common::int32_t newx,
                    batos::common::int32_t newy
                );
        };

        class CompositeWidget : public Widget {
            private:
                Widget* children[100];
                int numChildren;
                Widget* focusedChild;

            public:
                CompositeWidget(
                    Widget* parent,
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::int32_t w,
                    batos::common::int32_t h,
                    batos::common::int8_t r,
                    batos::common::int8_t g,
                    batos::common::int8_t b
                );
                ~CompositeWidget();

                virtual void GetFocus(Widget* widget);
                virtual bool AddChild(Widget* child);

                virtual void Draw(batos::common::GraphicsContext* gc);
                virtual void OnMouseDown(
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::uint8_t button
                );
                virtual void OnMouseUp(
                    batos::common::int32_t x,
                    batos::common::int32_t y,
                    batos::common::uint8_t button
                );
                virtual void OnMouseMove(
                    batos::common::int32_t oldx,
                    batos::common::int32_t oldy,
                    batos::common::int32_t newx,
                    batos::common::int32_t newy
                );

                virtual void OnKeyDown(char);
                virtual void OnKeyUp(char);
        };
    };  // namespace gui
}  // namespace batos

#endif