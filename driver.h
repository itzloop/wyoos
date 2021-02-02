#pragma once

class Driver
{
public:
    Driver();
    ~Driver();

    virtual void activate();
    virtual int reset();
    virtual void deactivate();
};

class DriverManager
{
private:
    Driver *drivers[265];
    int numDrivers;

public:
    DriverManager();
    void addDriver(Driver *);
    void activateAll();
};
