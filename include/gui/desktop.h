#pragma once
#include <drivers/mouse.h>
#include <gui/widget.h>

// this will draw the mouse and handle mouse events

namespace myos
{
    namespace gui
    {
        class Desktop : public CompositeWidget, public drivers::MouseEventHandler
        {
        protected:
            common::uint32_t mouseX;
            common::uint32_t mouseY;

        public:
            Desktop(common::int32_t w, common::int32_t h,
                    common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();

            void draw(common::GraphicsContext *gc);

            void onMouseDown(myos::common::uint8_t button);
            void onMouseUp(myos::common::uint8_t button);
            void onMouseMove(int x, int y);
        };
    } // namespace gui
} // namespace myos