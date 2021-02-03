#include <common/types.h>
#include <gdt.h>
#include <drivers/driver.h>
#include <hardwarecom/interrupts.h>
#include <hardwarecom/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>
#include <mmu.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;
using namespace myos::drivers;
using namespace myos::hardwarecoms;

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

// #define GRAPHICSMODE

void printf(char *str)
{
    static uint16_t *vm = (uint16_t *)0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;
        default:
            vm[SCREEN_WIDTH * y + x] = (vm[SCREEN_WIDTH * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }
        // if we reached end of the line
        if (x >= SCREEN_WIDTH)
        {
            x = 0;
            y++;
        }

        // if we reached end of the screen
        if (y >= SCREEN_HEIGHT)
        {
            for (y = 0; y < SCREEN_HEIGHT; ++y)
                for (x = 0; x < SCREEN_WIDTH; ++x)
                    vm[SCREEN_WIDTH * y + x] = (vm[SCREEN_WIDTH * y + x] & 0xFF00) | ' ';
            x = y = 0;
        }
    }
}

void printHex(uint8_t n)
{
    char *foo = "0x00";
    char *hex = "0123456789ABCDEF";
    foo[2] = hex[(n >> 4) & 0x0F];
    foo[3] = hex[n & 0x0F];
    printf(foo);
}

void printAddr(void *ptr)
{
    char *foo = "0x00000000\n";
    char *hex = "0123456789ABCDEF";
    foo[2] = hex[((uint32_t)(ptr) >> 28) & 0x0000000F];
    foo[3] = hex[((uint32_t)(ptr) >> 24) & 0x0000000F];
    foo[4] = hex[((uint32_t)(ptr) >> 20) & 0x0000000F];
    foo[5] = hex[((uint32_t)(ptr) >> 16) & 0x0000000F];
    foo[6] = hex[((uint32_t)(ptr) >> 12) & 0x0000000F];
    foo[7] = hex[((uint32_t)(ptr) >> 8) & 0x0000000F];
    foo[8] = hex[((uint32_t)(ptr) >> 4) & 0x0000000F];
    foo[9] = hex[((uint32_t)ptr) & 0x0000000F];
    printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void onKeyDown(char c)
    {
        char *foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;

public:
    MouseToConsole()
    {
    }

    virtual void onActivate()
    {
        uint16_t *VideoMemory = (uint16_t *)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }

    virtual void onMouseMove(int xoffset, int yoffset)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

        x += xoffset;
        if (x >= 80)
            x = 79;
        if (x < 0)
            x = 0;
        y += yoffset;
        if (y >= 25)
            y = 24;
        if (y < 0)
            y = 0;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }
};

void taskA()
{
    while (1)
        printf("A");
}

void taskB()
{
    while (1)
        printf("B");
}

typedef void (*ctor)();

extern "C" ctor start_ctors;
extern "C" ctor end_ctors;
extern "C" void call_constructors()
{
    for (ctor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
    printf("Hello\n");

    // instantiate the global descriptor table

    GDT gdt;
    size_t heap = 10 * 1024 * 1024;
    uint32_t *memupper = (uint32_t *)(((size_t)multiboot_structure) + 8);
    MMU mmu(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printAddr((void *)heap);

    void *allocated = mmu.malloc(1024);
    printf("\nallocated: ");
    printAddr(allocated);
    mmu.free(allocated);
    allocated = mmu.malloc(1024);
    printf("\nallocated: ");
    printAddr(allocated);

    printf("\n");

    TaskManager taskManager;
    // Task a(&gdt, taskA);
    // Task b(&gdt, taskB);
    // taskManager.addTask(&a);
    // taskManager.addTask(&b);

    // instantiate the interrupt descriptor table
    InterruptManager interrupts(&gdt, &taskManager);
    DriverManager driverManager;
    Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#ifdef GRAPHICSMODE
    KeyboardDriver kbd(&interrupts, &desktop);
    MouseDriver md(&interrupts, &desktop);
#else
    PrintfKeyboardEventHandler handler;
    KeyboardDriver kbd(&interrupts, &handler);
    MouseToConsole mouseHandler;
    MouseDriver md(&interrupts, &mouseHandler);
#endif

    driverManager.addDriver(&kbd);
    driverManager.addDriver(&md);
    PICController controller;
    controller.selectDrivers(&driverManager, &interrupts);
    driverManager.activateAll();

#ifdef GRAPHICSMODE
    VGA vga;
    vga.setMode(320, 200, 8);
    Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
    Window win2(&desktop, 100, 120, 30, 20, 0x00, 0xA8, 0x00);
    desktop.addChild(&win1);
    desktop.addChild(&win2);
#endif
    interrupts.activate();
    // vga.fillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8);
    while (1)
#ifdef GRAPHICSMODE
        desktop.draw(&vga);
#else
        ;
#endif
}