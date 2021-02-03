#pragma once

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>

namespace myos
{
    namespace gui
    {
        class Widget : public myos::drivers::KeyboardEventHandler
        {
        protected:
            Widget *parent;
            common::int32_t x;
            common::int32_t y;
            common::int32_t w;
            common::int32_t h;

            common::uint8_t r;
            common::uint8_t g;
            common::uint8_t b;
            bool focussable;

        public:
            Widget(
                Widget *parent,
                common::int32_t x,
                common::int32_t y,
                common::int32_t w,
                common::int32_t h,
                common::uint8_t r,
                common::uint8_t g,
                common::uint8_t b);

            ~Widget();

            virtual void getFocus(Widget *widget);
            virtual void modelToScreen(
                common::int32_t &x, common::int32_t &y);

            virtual void draw(common::GraphicsContext *gc);
            virtual bool contains(common::int32_t x, common::int32_t y);
            virtual void onMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void onMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void onMouseMove(
                common::int32_t oldx,
                common::int32_t oldy,
                common::int32_t newx,
                common::int32_t newy);
        };

        class CompositeWidget : public Widget
        {
        private:
            Widget *children[100];
            int numChildren;
            Widget *focussedChild;

        public:
            CompositeWidget(
                Widget *parent,
                common::int32_t x,
                common::int32_t y,
                common::int32_t w,
                common::int32_t h,
                common::uint8_t r,
                common::uint8_t g,
                common::uint8_t b);

            ~CompositeWidget();

            virtual void getFocus(Widget *widget);
            virtual bool addChild(Widget *child);
            virtual void draw(common::GraphicsContext *gc);
            virtual void onMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void onMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);
            virtual void onMouseMove(
                common::int32_t oldx,
                common::int32_t oldy,
                common::int32_t newx,
                common::int32_t newy);

            virtual void onKeyDown(char);
            virtual void onKeyUp(char);
        };

    } // namespace gui

} // namespace myos