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
            common::uint8_t *address;
            common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PCIDeviceDescriptor
        {
        public:
            common::uint32_t portBase;
            common::uint32_t interrupt;

            common::uint16_t bus;
            common::uint16_t device;
            common::uint16_t function;

            common::uint16_t vendor_id;
            common::uint16_t device_id;

            common::uint8_t class_id;
            common::uint8_t subclass_id;
            common::uint8_t interface_id;

            common::uint8_t revision;

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

            common::uint32_t read(
                common::uint16_t bus,
                common::uint16_t device,
                common::uint16_t function,
                common::uint32_t registeroffset);

            void write(
                common::uint16_t bus,
                common::uint16_t device,
                common::uint16_t function,
                common::uint32_t registeroffset,
                common::uint32_t value);

            bool deviceHasFunctions(
                common::uint16_t bus,
                common::uint16_t device);

            void selectDrivers(
                drivers::DriverManager *driverManager,
                InterruptManager *manager);
            drivers::Driver *getDriver(
                PCIDeviceDescriptor dev,
                InterruptManager *interrupts);
            PCIDeviceDescriptor getDeviceDescriptor(
                common::uint16_t bus,
                common::uint16_t device,
                common::uint16_t function);

            BaseAddressRegister getBaseAddressRegister(
                common::uint16_t bus,
                common::uint16_t device,
                common::uint16_t function,
                common::uint16_t bar);
        };
    } // namespace hardwarecoms
} // namespace myos