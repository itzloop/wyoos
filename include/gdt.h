#pragma once
#include <common/types.h>

// Global Descriptor Table

namespace myos
{
    class GDT
    {
    private:
        class SegmentDescriptor
        {
        private:
            myos::common::uint16_t limit_lo;
            myos::common::uint16_t base_lo;
            myos::common::uint8_t base_hi;
            myos::common::uint8_t type;
            myos::common::uint8_t flags_limit_hi;
            myos::common::uint8_t base_vhi;

        public:
            SegmentDescriptor(myos::common::uint32_t base,
                              myos::common::uint32_t limit,
                              myos::common::uint8_t type);
            myos::common::uint32_t base();
            myos::common::uint32_t limit();
        } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GDT();
        ~GDT();
        myos::common::uint16_t CodeSegmentSelector();
        myos::common::uint16_t DataSegmentSelector();
    };
} // namespace myos