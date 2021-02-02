#pragma once
#include <common/types.h>
#include <hardwarecom/port.h>
#include <gdt.h>

namespace myos
{
    namespace hardwarecoms
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            common::uint8_t intteruptNumber;
            InterruptManager *interruptManager;
            InterruptHandler(common::uint8_t interruptNumber, InterruptManager *manager);
            ~InterruptHandler();

        public:
            virtual common::uint32_t handle(common::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;

        protected:
            static InterruptManager *current;
            InterruptHandler *handlers[256];
            struct GateDescriptor
            {
                common::uint16_t handlerAddressLowBits;
                common::uint16_t gdt_codeSegmentSelector;
                common::uint8_t reserved;
                common::uint8_t access;
                common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                common::uint16_t size;
                common::uint32_t base;
            } __attribute__((packed));

            static void setInterruptDescriptorTableEntry(
                common::uint8_t interruptNumber,
                common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                common::uint8_t descriptorPrivilegeLevel,
                common::uint8_t descriptorType);

            // these ports are for speaking with
            // programmable interrupt controllers (pic)
            // there are 2 pics one master and one slave

            Port8Slow picMasterCommand;
            Port8Slow picMasterData;
            Port8Slow picSlaveCommand;
            Port8Slow picSlaveData;

        public:
            InterruptManager(GDT *gdt);
            ~InterruptManager();
            void activate();
            void deactivate();
            static common::uint32_t handleInterrupt(common::uint8_t int_num, common::uint32_t esp);
            common::uint32_t doHandleInterrupt(common::uint8_t int_num, common::uint32_t esp);
            static void ignoreInterruptRequest();
            static void handleInterruptRequest0x00();
            static void handleInterruptRequest0x01();
            static void handleInterruptRequest0x0C();
        };
    } // namespace hardwarecoms

} // namespace myos