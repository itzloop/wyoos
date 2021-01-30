void printf(char *str)
{
    unsigned short *vm = (unsigned short *)0xb8000;
    for (int i = 0; i < str[i] != '\0'; i++)
    {
        vm[i] = (vm[i] & 0xFF00) | str[i];
    }
}

extern "C" void kernelMain(void *multiboot_structure, unsigned int magicnumber)
{
    printf("Helloo\n");
    while (1)
        ;
}