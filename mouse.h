#pragma once
#include "types.h"
#include "driver.h"
#include "interrupts.h"
#include "port.h"

class MouseEventHandler
{
    int8_t x, y;

public:
    MouseEventHandler();

    virtual void onActivate();
    virtual void onMouseDown(uint8_t button);
    virtual void onMouseUp(uint8_t button);
    virtual void onMouseMove(int x, int y);
};

class MouseDriver : public InterruptHandler, public Driver
{
    uint8_t offset;
    uint8_t buffer[3];
    uint8_t buttons;
    Port8 dataport;
    Port8 commandport;
    MouseEventHandler *handler;

public:
    MouseDriver(InterruptManager *manager, MouseEventHandler *handler);
    ~MouseDriver();
    virtual uint32_t handle(uint32_t esp);
    virtual void activate();
};