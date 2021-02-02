
#include <common/types.h>
#include <hardwarecom/port.h>
#include <drivers/driver.h>

// Video Graphics Array (VGA)

namespace myos
{
    namespace drivers
    {

        class VGA
        {
        protected:
            hardwarecoms::Port8 miscPort;
            hardwarecoms::Port8 crtcIndexPort;
            hardwarecoms::Port8 crtcDataPort;
            hardwarecoms::Port8 sequencerIndexPort;
            hardwarecoms::Port8 sequencerDataPort;
            hardwarecoms::Port8 graphicsControllerIndexPort;
            hardwarecoms::Port8 graphicsControllerDataPort;
            hardwarecoms::Port8 attributeControllerIndexPort;
            hardwarecoms::Port8 attributeControllerReadPort;
            hardwarecoms::Port8 attributeControllerWritePort;
            hardwarecoms::Port8 attributeControllerResetPort;

            void writeRegisters(common::uint8_t *registers);
            common::uint8_t *getFrameBufferSegment();

            virtual common::uint8_t GetColorIndex(
                common::uint8_t r,
                common::uint8_t g,
                common::uint8_t b);

        public:
            VGA();
            ~VGA();

            virtual bool SupportsMode(
                common::uint32_t width,
                common::uint32_t height,
                common::uint32_t colordepth);

            virtual bool SetMode(
                common::uint32_t width,
                common::uint32_t height,
                common::uint32_t colordepth);

            virtual void PutPixel(
                common::uint32_t x,
                common::uint32_t y,
                common::uint8_t r,
                common::uint8_t g,
                common::uint8_t b);

            virtual void PutPixel(
                common::uint32_t x,
                common::uint32_t y,
                common::uint8_t colorIndex);
        };
    } // namespace drivers
} // namespace myos