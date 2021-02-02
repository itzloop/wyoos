#pragma once
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/port.h>

namespace myos
{
    namespace drivers
    {
        class MouseEventHandler
        {
            myos::common::int8_t x, y;

        public:
            MouseEventHandler();

            virtual void onActivate();
            virtual void onMouseDown(myos::common::uint8_t button);
            virtual void onMouseUp(myos::common::uint8_t button);
            virtual void onMouseMove(int x, int y);
        };

        class MouseDriver : public myos::hardwarecoms::InterruptHandler, public Driver
        {
            myos::common::uint8_t offset;
            myos::common::uint8_t buffer[3];
            myos::common::uint8_t buttons;
            myos::hardwarecoms::Port8 dataport;
            myos::hardwarecoms::Port8 commandport;
            MouseEventHandler *handler;

        public:
            MouseDriver(myos::hardwarecoms::InterruptManager *manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual myos::common::uint32_t handle(myos::common::uint32_t esp);
            virtual void activate();
        };
    } // namespace drivers
} // namespace myos