#include "types.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

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
    // instantiate the interrupt descriptor table
    InterruptManager interrupts(&gdt);
    KeyboardDriver kbd(&interrupts);
    interrupts.activate();

    while (1)
        ;
}