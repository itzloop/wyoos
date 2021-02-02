#pragma once

#include <common/types.h>

namespace myos
{
    namespace hardwarecoms
    {
        class Port
        {
        protected:
            common::uint16_t port;
            Port(common::uint16_t port);
            ~Port();
        };

        class Port8 : public Port
        {
        public:
            Port8(common::uint16_t port);
            ~Port8();
            virtual void write(common::uint8_t data);
            virtual common::uint8_t read();
        };
        class Port8Slow : public Port8
        {
        public:
            Port8Slow(common::uint16_t port);
            ~Port8Slow();
            virtual void write(common::uint8_t data);
        };
        class Port16 : public Port
        {
        public:
            Port16(common::uint16_t port);
            ~Port16();
            virtual void write(common::uint16_t data);
            virtual common::uint16_t read();
        };
        class Port32 : public Port
        {
        public:
            Port32(common::uint16_t port);
            ~Port32();
            virtual void write(common::uint32_t data);
            virtual common::uint32_t read();
        };
    } // namespace hardwarecoms
} // namespace myos