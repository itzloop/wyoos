#include <drivers/keyboard.h>

using namespace myos::hardwarecoms;
using namespace myos::drivers;
using namespace myos::common;

void printf(char *);
void printHex(uint8_t);
KeyboardEventHandler::KeyboardEventHandler()
{
}

void KeyboardEventHandler::onKeyDown(char)
{
}

void KeyboardEventHandler::onKeyUp(char)
{
}

KeyboardDriver::KeyboardDriver(InterruptManager *manager, KeyboardEventHandler *handler)
    : InterruptHandler(0x21, manager),
      dataport(0x60),
      commandport(0x64)
{
    this->handler = handler;
}
KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::activate()
{
    while (commandport.read() & 0x1)
        dataport.read();
    commandport.write(0xAE);
    commandport.write(0x20);
    uint8_t status = (dataport.read() | 1) & ~0x10;
    commandport.write(0x60);
    dataport.write(status);
    dataport.write(0xF4);
}

uint32_t KeyboardDriver::handle(uint32_t esp)
{
    uint8_t key = dataport.read();

    if (handler == 0)
        return esp;
    switch (key)
    {
    case 0x02:
        handler->onKeyDown('1');
        break;
    case 0x03:
        handler->onKeyDown('2');
        break;
    case 0x04:
        handler->onKeyDown('3');
        break;
    case 0x05:
        handler->onKeyDown('4');
        break;
    case 0x06:
        handler->onKeyDown('5');
        break;
    case 0x07:
        handler->onKeyDown('6');
        break;
    case 0x08:
        handler->onKeyDown('7');
        break;
    case 0x09:
        handler->onKeyDown('8');
        break;
    case 0x0A:
        handler->onKeyDown('9');
        break;
    case 0x0B:
        handler->onKeyDown('0');
        break;

    case 0x10:
        handler->onKeyDown('q');
        break;
    case 0x11:
        handler->onKeyDown('w');
        break;
    case 0x12:
        handler->onKeyDown('e');
        break;
    case 0x13:
        handler->onKeyDown('r');
        break;
    case 0x14:
        handler->onKeyDown('t');
        break;
    case 0x15:
        handler->onKeyDown('z');
        break;
    case 0x16:
        handler->onKeyDown('u');
        break;
    case 0x17:
        handler->onKeyDown('i');
        break;
    case 0x18:
        handler->onKeyDown('o');
        break;
    case 0x19:
        handler->onKeyDown('p');
        break;

    case 0x1E:
        handler->onKeyDown('a');
        break;
    case 0x1F:
        handler->onKeyDown('s');
        break;
    case 0x20:
        handler->onKeyDown('d');
        break;
    case 0x21:
        handler->onKeyDown('f');
        break;
    case 0x22:
        handler->onKeyDown('g');
        break;
    case 0x23:
        handler->onKeyDown('h');
        break;
    case 0x24:
        handler->onKeyDown('j');
        break;
    case 0x25:
        handler->onKeyDown('k');
        break;
    case 0x26:
        handler->onKeyDown('l');
        break;

    case 0x2C:
        handler->onKeyDown('y');
        break;
    case 0x2D:
        handler->onKeyDown('x');
        break;
    case 0x2E:
        handler->onKeyDown('c');
        break;
    case 0x2F:
        handler->onKeyDown('v');
        break;
    case 0x30:
        handler->onKeyDown('b');
        break;
    case 0x31:
        handler->onKeyDown('n');
        break;
    case 0x32:
        handler->onKeyDown('m');
        break;
    case 0x33:
        handler->onKeyDown(',');
        break;
    case 0x34:
        handler->onKeyDown('.');
        break;
    case 0x35:
        handler->onKeyDown('-');
        break;

    case 0x1C:
        handler->onKeyDown('\n');
        break;
    case 0x39:
        handler->onKeyDown(' ');
        break;

    case 0x45: // ignore numlock
        break;
    default:
        if (key >= 0x80)
            break;
        printf("0x");
        printHex(key);
        break;
    }

    return esp;
}
