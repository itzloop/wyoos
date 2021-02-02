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
            myos::common::uint8_t intteruptNumber;
            InterruptManager *interruptManager;
            InterruptHandler(myos::common::uint8_t interruptNumber, InterruptManager *manager);
            ~InterruptHandler();

        public:
            virtual myos::common::uint32_t handle(myos::common::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;

        protected:
            static InterruptManager *current;
            InterruptHandler *handlers[256];
            struct GateDescriptor
            {
                myos::common::uint16_t handlerAddressLowBits;
                myos::common::uint16_t gdt_codeSegmentSelector;
                myos::common::uint8_t reserved;
                myos::common::uint8_t access;
                myos::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                myos::common::uint16_t size;
                myos::common::uint32_t base;
            } __attribute__((packed));

            static void setInterruptDescriptorTableEntry(
                myos::common::uint8_t interruptNumber,
                myos::common::uint16_t codeSegmentSelectorOffset,
                void (*handler)(),
                myos::common::uint8_t descriptorPrivilegeLevel,
                myos::common::uint8_t descriptorType);

            // these ports are for speaking with
            // programmable interrupt controllers (pic)
            // there are 2 pics one master and one slave

            myos::hardwarecoms::Port8Slow picMasterCommand;
            myos::hardwarecoms::Port8Slow picMasterData;
            myos::hardwarecoms::Port8Slow picSlaveCommand;
            myos::hardwarecoms::Port8Slow picSlaveData;

        public:
            InterruptManager(GDT *gdt);
            ~InterruptManager();
            void activate();
            void deactivate();
            static myos::common::uint32_t handleInterrupt(myos::common::uint8_t int_num, myos::common::uint32_t esp);
            myos::common::uint32_t doHandleInterrupt(myos::common::uint8_t int_num, myos::common::uint32_t esp);
            static void ignoreInterruptRequest();
            static void handleInterruptRequest0x00();
            static void handleInterruptRequest0x01();
            static void handleInterruptRequest0x0C();
        };
    } // namespace hardwarecoms

} // namespace myos