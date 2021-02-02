#pragma once
#include "types.h"
#include "driver.h"
#include "interrupts.h"
#include "port.h"
class KeyboardEventHandler
{
public:
    KeyboardEventHandler();

    virtual void onKeyDown(char);
    virtual void onKeyUp(char);
};

class KeyboardDriver : public InterruptHandler, public Driver
{
    Port8 dataport;
    Port8 commandport;
    KeyboardEventHandler *handler;

public:
    KeyboardDriver(InterruptManager *manager, KeyboardEventHandler *handler);
    ~KeyboardDriver();
    virtual uint32_t handle(uint32_t esp);
    virtual void activate();
};