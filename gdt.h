#pragma once
#include "types.h"
// Global Descriptor Table

class GDT
{
private:
    class SegmentDescriptor
    {
    private:
        uint16_t limit_lo;
        uint16_t base_lo;
        uint8_t base_hi;
        uint8_t type;
        uint8_t flags_limit_hi;
        uint8_t flags_limit_lo;
        uint8_t base_vhi;

    public:
        SegmentDescriptor(uint32_t base,
                          uint32_t limit,
                          uint8_t type);
        uint32_t base();
        uint32_t limit();
    } __attribute__((packed));

    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

public:
    GDT();
    ~GDT();
    uint16_t CodeSegmentSelector();
    uint16_t DataSegmentSelector();
};