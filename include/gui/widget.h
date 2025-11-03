#ifndef __BATOS__GUI_WIDGET_H
#define __BATOS__GUI_WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>

namespace batos
{
    namespace gui {
        class Widget {
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
                virtual void MovelToScreen(batos::common::int32_t &x, batos::common::int32_t &y);

                virtual void Draw(GraphicsContext* gc);
                virtual void OnMouseDown(batos::common::int32_t x, batos::common::int32_t y);
                virtual void OnMouseUp(batos::common::int32_t x, batos::common::int32_t y);
                virtual void OnMouseMove(batos::common::int32_t oldx, batos::common::int32_t oldy, batos::common::int32_t newx, batos::common::int32_t newy);

                virtual void OnKeyDown(batos::common::int32_t x, batos::common::int32_t y);
                virtual void OnKeyUp(batos::common::int32_t x, batos::common::int32_t y);

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

                virtual void Draw(GraphicsContext* gc);
                virtual void OnMouseDown(batos::common::int32_t x, batos::common::int32_t y);
                virtual void OnMouseUp(batos::common::int32_t x, batos::common::int32_t y);
                virtual void OnMouseMove(batos::common::int32_t oldx, batos::common::int32_t oldy, batos::common::int32_t newx, batos::common::int32_t newy);

                virtual void OnKeyDown(char* str);
                virtual void OnKeyUp(char* str);
        };
    };
} // namespace batos


#endif