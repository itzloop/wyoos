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
            common::uint16_t limit_lo;
            common::uint16_t base_lo;
            common::uint8_t base_hi;
            common::uint8_t type;
            common::uint8_t flags_limit_hi;
            common::uint8_t base_vhi;

        public:
            SegmentDescriptor(common::uint32_t base,
                              common::uint32_t limit,
                              common::uint8_t type);
            common::uint32_t base();
            common::uint32_t limit();
        } __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GDT();
        ~GDT();
        common::uint16_t CodeSegmentSelector();
        common::uint16_t DataSegmentSelector();
    };
} // namespace myos