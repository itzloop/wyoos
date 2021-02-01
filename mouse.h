#pragma once
#include "types.h"
#include "interrupts.h"
#include "port.h"

class MouseDriver : public InterruptHandler
{
    uint8_t offset;
    uint8_t buffer[3];
    uint8_t buttons;
    int8_t x, y;
    Port8 dataport;
    Port8 commandport;

public:
    MouseDriver(InterruptManager *manager);
    ~MouseDriver();
    virtual uint32_t handle(uint32_t esp);
};