#pragma once

#include "types.h"

class Port
{
protected:
    uint16_t port;
    Port(uint16_t port);
    ~Port();
};

class Port8 : public Port
{
public:
    Port8(uint8_t port);
    ~Port8();
    virtual void write(uint8_t data);
    virtual uint8_t read(uint8_t data);
};
class Port8Slow : public Port8
{
public:
    Port8Slow(uint8_t port);
    ~Port8Slow();
    virtual void write(uint8_t data);
};
class Port16 : public Port
{
public:
    Port16(uint16_t port);
    ~Port16();
    virtual void write(uint16_t data);
    virtual uint16_t read(uint16_t data);
};
class Port32 : public Port
{
public:
    Port32(uint32_t port);
    ~Port32();
    virtual void write(uint32_t data);
    virtual uint32_t read(uint32_t data);
};