#include <gui/desktop.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;

Desktop::Desktop(common::int32_t w, common::int32_t h,
                 common::uint8_t r, common::uint8_t g, common::uint8_t b)
    : CompositeWidget(0, 0, 0, w, h, r, g, b)
{
    mouseX = w / 2;
    mouseY = h / 2;
}
Desktop::~Desktop() {}

void Desktop::draw(common::GraphicsContext *gc)
{
    CompositeWidget::draw(gc);
    for (int i = 0; i < 4; ++i)
    {
        gc->putPixel(mouseX - i, mouseY, 0xFF, 0xFF, 0xFF);
        gc->putPixel(mouseX + i, mouseY, 0xFF, 0xFF, 0xFF);
        gc->putPixel(mouseX, mouseY - i, 0xFF, 0xFF, 0xFF);
        gc->putPixel(mouseX, mouseY + i, 0xFF, 0xFF, 0xFF);
    }
}

void Desktop::onMouseDown(myos::common::uint8_t button)
{
    CompositeWidget::onMouseDown(mouseX, mouseY, button);
}
void Desktop::onMouseUp(myos::common::uint8_t button)
{
    CompositeWidget::onMouseUp(mouseX, mouseY, button);
}
void Desktop::onMouseMove(int x, int y)
{
    x /= 4;
    y /= 4;
    int32_t newMouseX = mouseX + x;
    if (newMouseX < 0)
        newMouseX = 0;
    if (newMouseX >= w)
        newMouseX = w - 1;

    int32_t newMouseY = mouseY + y;
    if (newMouseY < 0)
        newMouseY = 0;
    if (newMouseY >= h)
        newMouseY = h - 1;

    CompositeWidget::onMouseMove(mouseX, mouseY, newMouseX, newMouseY);

    mouseX = newMouseX;
    mouseY = newMouseY;
}