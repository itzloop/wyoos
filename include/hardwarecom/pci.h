#pragma once
#include <hardwarecom/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecom/interrupts.h>
// peripheral component Interconnect controller

namespace myos
{
    namespace hardwarecoms
    {

        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister
        {
        public:
            bool prefetchable; // 4th bit used for memory mapping
            myos::common::uint8_t *address;
            myos::common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PCIDeviceDescriptor
        {
        public:
            myos::common::uint32_t portBase;
            myos::common::uint32_t interrupt;

            myos::common::uint16_t bus;
            myos::common::uint16_t device;
            myos::common::uint16_t function;

            myos::common::uint16_t vendor_id;
            myos::common::uint16_t device_id;

            myos::common::uint8_t class_id;
            myos::common::uint8_t subclass_id;
            myos::common::uint8_t interface_id;

            myos::common::uint8_t revision;

            PCIDeviceDescriptor();
            ~PCIDeviceDescriptor();
        };

        class PICController
        {
        public:
            Port32 dataport;
            Port32 commandport;

            PICController();
            ~PICController();

            myos::common::uint32_t read(
                myos::common::uint16_t bus,
                myos::common::uint16_t device,
                myos::common::uint16_t function,
                myos::common::uint32_t registeroffset);

            void write(
                myos::common::uint16_t bus,
                myos::common::uint16_t device,
                myos::common::uint16_t function,
                myos::common::uint32_t registeroffset,
                myos::common::uint32_t value);

            bool deviceHasFunctions(
                myos::common::uint16_t bus,
                myos::common::uint16_t device);

            void selectDrivers(
                myos::drivers::DriverManager *driverManager,
                myos::hardwarecoms::InterruptManager *manager);
            myos::drivers::Driver *getDriver(
                PCIDeviceDescriptor dev,
                myos::hardwarecoms::InterruptManager *interrupts);
            PCIDeviceDescriptor getDeviceDescriptor(
                myos::common::uint16_t bus,
                myos::common::uint16_t device,
                myos::common::uint16_t function);

            BaseAddressRegister getBaseAddressRegister(
                myos::common::uint16_t bus,
                myos::common::uint16_t device,
                myos::common::uint16_t function,
                myos::common::uint16_t bar);
        };
    } // namespace hardwarecoms
} // namespace myos