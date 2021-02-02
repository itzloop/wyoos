#pragma once
#include <common/types.h>
#include <drivers/driver.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/port.h>

namespace myos
{
    namespace drivers
    {

        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void onKeyDown(char);
            virtual void onKeyUp(char);
        };

        class KeyboardDriver : public hardwarecoms::InterruptHandler, public Driver
        {
            hardwarecoms::Port8 dataport;
            hardwarecoms::Port8 commandport;
            KeyboardEventHandler *handler;

        public:
            KeyboardDriver(hardwarecoms::InterruptManager *manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual common::uint32_t handle(common::uint32_t esp);
            virtual void activate();
        };
    } // namespace drivers
} // namespace myos