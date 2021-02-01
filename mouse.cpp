#include "mouse.h"
void printf(char *);

MouseDriver::MouseDriver(InterruptManager *manager)
    : InterruptHandler(0x2C, manager),
      dataport(0x60),
      commandport(0x64)
{
    offset = 0;
    buttons = 0;
    x = 40;
    y = 12;
    static uint16_t *vm = (uint16_t *)0xb8000;
    vm[80 * y + x] = (vm[80 * y + x] & 0x0F00) << 4 |
                     (vm[80 * y + x] & 0xF000) >> 4 |
                     (vm[80 * y + x] & 0x00FF);
    commandport.write(0xA8);
    commandport.write(0x20);
    uint8_t status = dataport.read() | 2;
    commandport.write(0x60);
    dataport.write(status);
    commandport.write(0xD4);
    dataport.write(0xF4);
    dataport.read();
}
MouseDriver::~MouseDriver() {}
uint32_t MouseDriver::handle(uint32_t esp)
{
    uint8_t status = commandport.read();
    if (!(status & 0x20))
        return esp;

    buffer[offset] = dataport.read();
    offset = (offset + 1) % 3;

    if (offset == 0)
    {
        static uint16_t *vm = (uint16_t *)0xb8000;
        vm[80 * y + x] = (vm[80 * y + x] & 0x0F00) << 4 |
                         (vm[80 * y + x] & 0xF000) >> 4 |
                         (vm[80 * y + x] & 0x00FF);

        x += buffer[1];
        if (x < 0)
            x = 0;
        if (x >= 80)
            x = 79;

        y -= buffer[2];
        if (y < 0)
            y = 0;
        if (y >= 25)
            y = 24;
        vm[80 * y + x] = (vm[80 * y + x] & 0x0F00) << 4 |
                         (vm[80 * y + x] & 0xF000) >> 4 |
                         (vm[80 * y + x] & 0x00FF);
    }
    return esp;
}
