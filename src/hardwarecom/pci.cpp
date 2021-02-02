#include <hardwarecom/pci.h>
#include <common/types.h>
#include <drivers/driver.h>

using namespace myos::common;
using namespace myos::hardwarecoms;
using namespace myos::drivers;

BaseAddressRegister PICController::getBaseAddressRegister(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint16_t bar)
{
    BaseAddressRegister result;

    uint32_t headertype = read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4 * headertype);
    if (bar >= maxBARs)
        return result;

    uint32_t bar_value = read(bus, device, function, 0x10 + 4 * bar);

    // check the lastbit for type
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;

    if (result.type == MemoryMapping)
    {
        // we don't care about this part ¯\_(ツ)_/¯
        switch ((bar_value >> 1) & 0x3)
        {

        case 0: // 32 Bit Mode
        case 1: // 20 Bit Mode
        case 2: // 64 Bit Mode
            break;
        }
    }
    else // InputOutput
    {
        result.address = (uint8_t *)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
}

PCIDeviceDescriptor::PCIDeviceDescriptor() {}
PCIDeviceDescriptor::~PCIDeviceDescriptor() {}

PICController::PICController()
    : dataport(0xCFC), commandport(0xCF8)
{
}
PICController::~PICController() {}

uint32_t PICController::read(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31 |
        (bus & 0xFF) << 16 |
        (device & 0x1F) << 11 |
        (function & 0x07) << 8 |
        (registeroffset & 0xFC);

    commandport.write(id);
    uint32_t result = dataport.read();
    return result >> (8 * (registeroffset % 4));
}
void PICController::write(
    uint16_t bus,
    uint16_t device,
    uint16_t function,
    uint32_t registeroffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31 |
        (bus & 0xFF) << 16 |
        (device & 0x1F) << 11 |
        (function & 0x07) << 8 |
        (registeroffset & 0xFC);
    commandport.write(id);
    dataport.write(value);
}
bool PICController::deviceHasFunctions(
    uint16_t bus,
    uint16_t device)
{
    return read(bus, device, 0, 0x0E) & (1 << 7);
}

void printf(char *str);
void printAddr(void *ptr);
void printHex(uint8_t n);

void PICController::selectDrivers(
    DriverManager *driverManager,
    InterruptManager *manager)
{
    for (int bus = 0; bus < 8; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int functionCount =
                deviceHasFunctions(bus, device) ? 1 : 8;

            for (int function = 0; function < functionCount; function++)
            {
                auto dev = getDeviceDescriptor(bus, device, function);

                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                for (int barNum = 0; barNum < 6; barNum++)
                {
                    auto bar = getBaseAddressRegister(bus, device, function, barNum);
                    if (bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;

                    auto *driver = getDriver(dev, manager);
                    if (driver != 0)
                        driverManager->addDriver(driver);
                }

                printf("PCI BUS ");
                printHex(bus & 0xFF);

                printf(", DEVICE ");
                printHex(device & 0xFF);

                printf(", FUNCTION ");
                printHex(function & 0xFF);

                printf(" = VENDOR ");
                printHex((dev.vendor_id & 0xFF00) >> 8);
                printHex(dev.vendor_id & 0xFF);
                printf(", DEVICE ");
                printHex((dev.device_id & 0xFF00) >> 8);
                printHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

Driver *PICController::getDriver(
    PCIDeviceDescriptor dev,
    myos::hardwarecoms::InterruptManager *interrupts)
{

    switch (dev.vendor_id)
    {
    case 0x1022: // AMD
        switch (dev.device_id)
        {
        case 0x2000: // am79c973
            printf("AMD am79c973 ");
            break;
        }
        break;

    case 0x8086: // Intel
        break;
    }

    switch (dev.class_id)
    {
    case 0x03: // graphics
        switch (dev.subclass_id)
        {
        case 0x00: // VGA
            printf("VGA ");
            break;
        }
        break;
    }

    return 0;
}

PCIDeviceDescriptor PICController::getDeviceDescriptor(
    uint16_t bus,
    uint16_t device,
    uint16_t function)
{
    PCIDeviceDescriptor result;
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = read(bus, device, function, 0x00);
    result.device_id = read(bus, device, function, 0x02);

    result.class_id = read(bus, device, function, 0x0B);
    result.subclass_id = read(bus, device, function, 0x0A);
    result.interface_id = read(bus, device, function, 0x09);

    result.revision = read(bus, device, function, 0x08);
    result.interrupt = read(bus, device, function, 0x3C);

    return result;
}
