#include <gui/widget.h>

using namespace myos::gui;

Widget::Widget(
    Widget *parent,
    myos::common::int32_t x,
    myos::common::int32_t y,
    myos::common::int32_t w,
    myos::common::int32_t h,
    myos::common::uint8_t r,
    myos::common::uint8_t g,
    myos::common::uint8_t b)
{

    this->parent = parent;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->r = r;
    this->g = g;
    this->b = b;
    this->focussable = true;
}

Widget::~Widget() {}

void Widget::getFocus(Widget *widget)
{
    if (parent != 0)
        parent->getFocus(widget);
}
void Widget::modelToScreen(
    myos::common::int32_t &x, myos::common::int32_t &y)
{
    if (parent != 0)
        parent->modelToScreen(x, y);
    x += this->x;
    y += this->y;
}

void Widget::draw(myos::common::GraphicsContext *gc)
{
    int X = 0;
    int Y = 0;
    modelToScreen(X, Y);
    gc->fillRectangle(X, Y, w, h, r, g, b);
}

void Widget::onMouseDown(myos::common::int32_t x, myos::common::int32_t y, common::uint8_t button)
{

    if (focussable)
        getFocus(this);
}
void Widget::onMouseUp(myos::common::int32_t x, myos::common::int32_t y, common::uint8_t button) {}
void Widget::onMouseMove(
    myos::common::int32_t oldx,
    myos::common::int32_t oldy,
    myos::common::int32_t newx,
    myos::common::int32_t newy) {}

Widget *children[100];
int numChildren;
Widget *focussedChild;

CompositeWidget::CompositeWidget(
    Widget *parent,
    myos::common::int32_t x, myos::common::int32_t y,
    myos::common::int32_t w, myos::common::int32_t h,
    myos::common::uint8_t r, myos::common::uint8_t g, myos::common::uint8_t b)
    : Widget(parent, x, y, w, h, r, g, b)
{
    focussedChild = 0;
    numChildren = 0;
}

CompositeWidget::~CompositeWidget() {}
bool CompositeWidget::addChild(Widget *child)
{
    if (numChildren >= 100)
        return false;
    children[numChildren++] = child;
    return true;
}

void CompositeWidget::getFocus(Widget *widget)
{
    this->focussedChild = widget;
    if (parent != 0)
        parent->getFocus(this);
}

void CompositeWidget::draw(myos::common::GraphicsContext *gc)
{
    Widget::draw(gc);
    for (int i = numChildren - 1; i >= 0; --i)
        children[i]->draw(gc);
}
void CompositeWidget::onMouseDown(myos::common::int32_t x, myos::common::int32_t y, common::uint8_t button)
{
    for (int i = 0; i < numChildren; ++i)
        if (children[i]->contains(x - this->x, y - this->y))
        {
            children[i]->onMouseDown(x - this->x, y - this->y, button);
            break;
        }
}

void CompositeWidget::onMouseUp(myos::common::int32_t x, myos::common::int32_t y, common::uint8_t button)
{
    for (int i = 0; i < numChildren; ++i)
        if (children[i]->contains(x - this->x, y - this->y))
        {
            children[i]->onMouseUp(x - this->x, y - this->y, button);
            break;
        }
}
void CompositeWidget::onMouseMove(
    myos::common::int32_t oldx,
    myos::common::int32_t oldy,
    myos::common::int32_t newx,
    myos::common::int32_t newy)
{
    int firstchild = -1;
    for (int i = 0; i < numChildren; ++i)
        if (children[i]->contains(oldx - this->x, oldy - this->y))
        {
            children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            firstchild = i;
            break;
        }

    for (int i = 0; i < numChildren; ++i)
        if (children[i]->contains(newx - this->x, newy - this->y))
        {
            if (firstchild != i)
                children[i]->onMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            break;
        }
}

bool Widget::contains(myos::common::int32_t x, myos::common::int32_t y)
{
    return this->x <= x && x < this->x + this->w &&
           this->y <= y && y < this->y + this->h;
}

void CompositeWidget::onKeyDown(char str)
{
    if (focussedChild != 0)
        focussedChild->onKeyDown(str);
}
void CompositeWidget::onKeyUp(char str)
{
    if (focussedChild != 0)
        focussedChild->onKeyUp(str);
}