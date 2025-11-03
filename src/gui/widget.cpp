#include <gui/widget.h>

using namespace batos::common;
using namespace batos::gui;

Widget::Widget(
    Widget* parent, 
    int32_t x,
    int32_t y,
    int32_t w,
    int32_t h,
    int8_t r,
    int8_t g,
    int8_t b
) {
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->isFocusable = true;
};

Widget::~Widget() {};

void Widget::GetFocus(Widget* widget) {
    if (parent != 0) {
        parent->GetFocus(widget);
    }
};

void Widget::MovelToScreen(int32_t &x, int32_t &y) {
    if (parent != 0) {
        parent->MovelToScreen(x, y);
    }
    x += this->x;
    y += this->y;
};

void Widget::Draw(GraphicsContext* gc) {
    int X = 0;
    int Y = 0;
    MovelToScreen(X, Y);
    gc->FillRectangle(X, Y, w, h, r, g, b);
};

void Widget::OnMouseDown(int32_t x, int32_t y) {
    if (isFocusable) {
        GetFocus(this);
    }
};

void Widget::OnMouseUp(int32_t x, int32_t y) {};

void Widget::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy) {};

void Widget::OnKeyDown(int32_t x, int32_t y) {};

void Widget::OnKeyUp(int32_t x, int32_t y) {};

CompositeWidget::CompositeWidget(
    Widget* parent, 
    batos::common::int32_t x,
    batos::common::int32_t y,
    batos::common::int32_t w,
    batos::common::int32_t h,
    batos::common::int8_t r,
    batos::common::int8_t g,
    batos::common::int8_t b
) {};

CompositeWidget::~CompositeWidget() {
    focusedChild = 0;
    numChildren = 0;
};

void CompositeWidget::GetFocus(Widget* widget) {
    this->focusedChild = widget;
    if (parent != 0) {
        parent->GetFocus(this);
    }

};

void CompositeWidget::Draw(GraphicsContext* gc) {
    Widget::Draw(gc); // its own background first
    for (int i = numChildren - 1; i >= 0; --i) {
        children[i]->Draw(gc);
    }
};

void CompositeWidget::OnMouseDown(batos::common::int32_t x, batos::common::int32_t y) {
    for (int i = 0; i < numChildren; ++i) {
        if (children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
            children[i]->OnMouseUp(x - this->x, y - this->y);
            break;
        };
    }
};

void CompositeWidget::OnMouseUp(batos::common::int32_t x, batos::common::int32_t y) {
    for (int i = 0; i < numChildren; ++i) {
        if (children[i]->ContainsCoordinate(x - this->x, y - this->y)) {
            children[i]->OnMouseDown(x - this->x, y - this->y);
            break;
        };
    }
};

void CompositeWidget::OnMouseMove(batos::common::int32_t oldx, batos::common::int32_t oldy, batos::common::int32_t newx, batos::common::int32_t newy) {
    int firstchild = -1;

    for (int i = 0; i < numChildren; ++i) {
        if (children[i]->ContainsCoordinate(oldx - this->x, oldy - this->y)) {
            children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            firstchild = i;
            break;
        };
    }

    for (int i = 0; i < numChildren; ++i) {
        if (children[i]->ContainsCoordinate(newx - this->x, newy - this->y)) {
            if (firstchild != i) {
                children[i]->OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            }
            break;
        };
    }
};

void CompositeWidget::OnKeyDown(char* str) {
    if (focusedChild != 0) {
        focusedChild->OnKeyDown(str);
    }
};

void CompositeWidget::OnKeyUp(char* str) {
    if (focusedChild != 0) {
        focusedChild->OnKeyDown(str);
    }
};
