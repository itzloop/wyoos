#include <hardwarecom/port.h>

using namespace myos::hardwarecoms;
using namespace myos::common;

// port base class
Port::Port(uint16_t port)
{
    this->port = port;
}
Port::~Port() {}

// 8 bit port implemetation
Port8::Port8(uint16_t port) : Port(port) {}
Port8::~Port8() {}
void Port8::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1"
                     :
                     : "a"(data), "Nd"(port));
}
uint8_t Port8::read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0"
                     : "=a"(result)
                     : "Nd"(port));
    return result;
}

// 8 bit slow port implemetation
Port8Slow::Port8Slow(uint16_t port) : Port8(port) {}
Port8Slow::~Port8Slow() {}
void Port8Slow::write(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:"
                     :
                     : "a"(data), "Nd"(port));
}

// 16 bit port implemetation

Port16::Port16(uint16_t port) : Port(port) {}
Port16::~Port16() {}
void Port16::write(uint16_t data)
{
    __asm__ volatile("outw %0, %1"
                     :
                     : "a"(data), "Nd"(port));
}
uint16_t Port16::read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0"
                     : "=a"(result)
                     : "Nd"(port));
    return result;
}

// 32 bit port implemetation
Port32::Port32(uint16_t port) : Port(port) {}
Port32::~Port32() {}
void Port32::write(uint32_t data)
{
    __asm__ volatile("outl %0, %1"
                     :
                     : "a"(data), "Nd"(port));
}
uint32_t Port32::read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0"
                     : "=a"(result)
                     : "Nd"(port));
    return result;
}
