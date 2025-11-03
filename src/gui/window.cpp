#include <gui/window.h>

using namespace batos::common;
using namespace batos::gui; 

Window::Window(
    Widget* parent, 
    batos::common::int32_t x,
    batos::common::int32_t y,
    batos::common::int32_t w,
    batos::common::int32_t h,
    batos::common::int8_t r,
    batos::common::int8_t g,
    batos::common::int8_t b
): CompositeWidget(parent, x, y, w, h, r, g, b) {
    Dragging = false;
};

Window::~Window() {};

void Window::OnMouseDown(batos::common::int32_t x, batos::common::int32_t y, batos::common::uint8_t button) {
    Dragging = button == 1;
    CompositeWidget::OnMouseDown(x, y, button);
};

void Window::OnMouseUp(batos::common::int32_t x, batos::common::int32_t y, batos::common::uint8_t button) {
    Dragging = false;
    CompositeWidget::OnMouseUp(x, y, button);
};

void Window::OnMouseMove(batos::common::int32_t oldx, batos::common::int32_t oldy, batos::common::int32_t newx, batos::common::int32_t newy) {
    if (Dragging) {
        this->x += newx - oldx;
        this->y += newy - oldy;
    }
    CompositeWidget::OnMouseMove(oldx, oldy, newx, newy);
};
