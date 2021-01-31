#include "types.h"
#include "gdt.h"

void printf(char *str)
{
    uint16_t *vm = (uint16_t *)0xb8000;
    for (int i = 0; i < str[i] != '\0'; i++)
    {
        vm[i] = (vm[i] & 0xFF00) | str[i];
    }
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
    printf("Helloo\n");

    // instantiate the global descriptor table
    GDT gdt;

    while (1)
        ;
}