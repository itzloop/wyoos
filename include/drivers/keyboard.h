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

        class KeyboardDriver : public myos::hardwarecoms::InterruptHandler, public Driver
        {
            myos::hardwarecoms::Port8 dataport;
            myos::hardwarecoms::Port8 commandport;
            KeyboardEventHandler *handler;

        public:
            KeyboardDriver(myos::hardwarecoms::InterruptManager *manager, KeyboardEventHandler *handler);
            ~KeyboardDriver();
            virtual myos::common::uint32_t handle(myos::common::uint32_t esp);
            virtual void activate();
        };
    } // namespace drivers
} // namespace myos