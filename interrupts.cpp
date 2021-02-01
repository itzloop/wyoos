#include "interrupts.h"

void printf(char *str);
void printAddr(void *ptr);

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager *InterruptManager::current = 0;

void InterruptManager::setInterruptDescriptorTableEntry(
    uint8_t interruptNumber,
    uint16_t codeSegmentSelectorOffset,
    void (*handler)(),
    uint8_t descriptorPrivilegeLevel,
    uint8_t descriptorType)
{
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | ((descriptorPrivilegeLevel & 3) << 5) | descriptorType;
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(GDT *gdt)
    : picMasterCommand(0x20),
      picMasterData(0x21),
      picSlaveCommand(0xA0),
      picSlaveData(0xA1)
{
    uint16_t codeSegment = gdt->CodeSegmentSelector();
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t i = 0; i < 256; ++i)
    {
        handlers[i] = 0;
        setInterruptDescriptorTableEntry(i, codeSegment, &ignoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
    }

    setInterruptDescriptorTableEntry(0x20, codeSegment, &handleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x21, codeSegment, &handleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x2C, codeSegment, &handleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.write(0x11);
    picSlaveCommand.write(0x11);

    picMasterData.write(0x20);
    picSlaveData.write(0x28);

    picMasterData.write(0x04);
    picSlaveData.write(0x02);

    picMasterData.write(0x01);
    picSlaveData.write(0x01);

    picMasterData.write(0x00);
    picSlaveData.write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0"
                 :
                 : "m"(idt));
}
InterruptManager::~InterruptManager() {}

void InterruptManager::activate()
{
    if (current != 0)
        current->deactivate();
    current = this;

    asm("sti");
}
void InterruptManager::deactivate()
{
    if (current == this)
    {
        current = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::handleInterrupt(uint8_t int_num, uint32_t esp)
{
    if (current != 0)
    {
        return current->doHandleInterrupt(int_num, esp);
    }
    return esp;
}
uint32_t InterruptManager::doHandleInterrupt(uint8_t int_num, uint32_t esp)
{
    if (handlers[int_num] != 0)
    {
        esp = (handlers[int_num]->handle(esp));
    }
    else if (int_num != 0x20)
    {
        char *foo = "UNHANDLED INTERRUPT 0x00";
        char *hex = "0123456789ABCDEF";
        foo[22] = hex[(int_num >> 4) & 0x0F];
        foo[23] = hex[int_num & 0x0F];
        printf(foo);
    }

    if (0x20 <= int_num && int_num < 0x30)
    {
        picMasterCommand.write(0x20);
        if (0x28 <= int_num)
            picSlaveCommand.write(0x20);
    }
    return esp;
}

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager *manager)
{
    char *foo = "Setting Interrupt 0x00";
    char *hex = "0123456789ABCDEF";
    foo[20] = hex[(interruptNumber >> 4) & 0x0F];
    foo[21] = hex[interruptNumber & 0x0F];
    printf(foo);
    this->intteruptNumber = intteruptNumber;
    this->interruptManager = manager;
    manager->handlers[interruptNumber] = this;
}
InterruptHandler::~InterruptHandler()
{
    if (interruptManager->handlers[intteruptNumber] == this)
        interruptManager->handlers[intteruptNumber] = 0;
}

uint32_t InterruptHandler::handle(uint32_t esp)
{
    return esp;
}