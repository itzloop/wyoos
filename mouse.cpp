#include "mouse.h"
void printf(char *);

MouseDriver::MouseDriver(InterruptManager *manager, MouseEventHandler *handler)
    : InterruptHandler(0x2C, manager),
      dataport(0x60),
      commandport(0x64)
{
    this->handler = handler;
}

MouseEventHandler::MouseEventHandler()
{
}

void MouseEventHandler::onActivate()
{
}

void MouseEventHandler::onMouseDown(uint8_t button)
{
}

void MouseEventHandler::onMouseUp(uint8_t button)
{
}

void MouseEventHandler::onMouseMove(int x, int y)
{
}

MouseDriver::~MouseDriver() {}
void MouseDriver::activate()
{
    offset = 0;
    buttons = 0;
    commandport.write(0xA8);
    commandport.write(0x20);
    uint8_t status = dataport.read() | 2;
    commandport.write(0x60);
    dataport.write(status);
    commandport.write(0xD4);
    dataport.write(0xF4);
    dataport.read();
}
uint32_t MouseDriver::handle(uint32_t esp)
{
    uint8_t status = commandport.read();
    if (!(status & 0x20))
        return esp;

    buffer[offset] = dataport.read();

    if (handler == 0)
        return esp;

    offset = (offset + 1) % 3;

    if (offset == 0)
    {
        handler->onMouseMove(buffer[1], -buffer[2]);
    }
    return esp;
}
