#include <gui/desktop.h>

using namespace batos;
using namespace batos::common;
using namespace batos::gui;

Desktop::Desktop(
    batos::common::int32_t w,
    batos::common::int32_t h,
    batos::common::uint8_t r,
    batos::common::uint8_t g,
    batos::common::uint8_t b
)
    : CompositeWidget(0, 0, 0, w, h, r, g, b), MouseEventHandler() {
    MouseX = w / 2;
    MouseY = h / 2;
};

Desktop::~Desktop() {};

void Desktop::Draw(batos::common::GraphicsContext* gc) {
    CompositeWidget::Draw(gc);

    for (int i = 0; i < 4; i++) {
        gc->PutPixel(MouseX - i, MouseY, 0xFF, 0xFF, 0xFF);
        gc->PutPixel(MouseX + i, MouseY, 0xFF, 0xFF, 0xFF);
        gc->PutPixel(MouseX, MouseY - i, 0xFF, 0xFF, 0xFF);
        gc->PutPixel(MouseX, MouseY + i, 0xFF, 0xFF, 0xFF);
    }
};

void Desktop::OnMouseDown(batos::common::uint8_t button) {
    CompositeWidget::OnMouseDown(MouseX, MouseY, button);
};

void Desktop::OnMouseUp(batos::common::uint8_t button) {
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);
};

void Desktop::OnMouseMove(batos::common::int8_t x, batos::common::int8_t y) {
    // Reduce sensitivity
    x /= 4;
    y /= 4;

    int32_t newMouseX = MouseX + x;
    if (newMouseX < 0) newMouseX = 0;
    if (newMouseX >= w) newMouseX = w - 1;

    int32_t newMouseY = MouseY + y;
    if (newMouseY < 0) newMouseY = 0;
    if (newMouseY >= h) newMouseY = h - 1;

    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);

    MouseX = newMouseX;
    MouseY = newMouseY;
};