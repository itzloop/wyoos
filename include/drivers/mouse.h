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
            common::int8_t x, y;

        public:
            MouseEventHandler();

            virtual void onActivate();
            virtual void onMouseDown(common::uint8_t button);
            virtual void onMouseUp(common::uint8_t button);
            virtual void onMouseMove(int x, int y);
        };

        class MouseDriver : public hardwarecoms::InterruptHandler, public Driver
        {
            common::uint8_t offset;
            common::uint8_t buffer[3];
            common::uint8_t buttons;
            hardwarecoms::Port8 dataport;
            hardwarecoms::Port8 commandport;
            MouseEventHandler *handler;

        public:
            MouseDriver(hardwarecoms::InterruptManager *manager, MouseEventHandler *handler);
            ~MouseDriver();
            virtual common::uint32_t handle(common::uint32_t esp);
            virtual void activate();
        };
    } // namespace drivers
} // namespace myos