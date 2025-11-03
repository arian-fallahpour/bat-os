#ifndef __BATOS__DRIVERS__VGA_H
#define __BATOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace batos {
    namespace drivers {
        class VideoGraphicsArray {
            protected:
                batos::hardwarecommunication::Port8Bit miscPort;
                batos::hardwarecommunication::Port8Bit crtcIndexPort;
                batos::hardwarecommunication::Port8Bit crtcDataPort;
                batos::hardwarecommunication::Port8Bit seqencerIndexPort;
                batos::hardwarecommunication::Port8Bit sequencerDataPort;
                batos::hardwarecommunication::Port8Bit graphicsControllerIndexPort;
                batos::hardwarecommunication::Port8Bit graphicsControllerDataPort;
                batos::hardwarecommunication::Port8Bit attributeControllerIndexPort;
                batos::hardwarecommunication::Port8Bit attributeControllerReadPort;
                batos::hardwarecommunication::Port8Bit attributeControllerWritePort;
                batos::hardwarecommunication::Port8Bit attributeControllerResetPort;

                void WriteRegisters(batos::common::uint8_t* registers);
                batos::common::uint8_t* GetFrameBufferSegment();
                
                virtual batos::common::int8_t GetColorIndex(batos::common::uint8_t r, batos::common::uint8_t g, batos::common::uint8_t b);
                public:
                VideoGraphicsArray();
                ~VideoGraphicsArray();
                
                virtual bool SupportsMode(batos::common::int32_t width, batos::common::int32_t height, batos::common::int32_t colordepth);
                virtual bool SetMode(batos::common::int32_t width, batos::common::int32_t height, batos::common::int32_t colordepth);
                virtual void PutPixel(batos::common::int32_t x, batos::common::int32_t y, batos::common::uint8_t r, batos::common::uint8_t g, batos::common::uint8_t b);
                virtual void PutPixel(batos::common::int32_t x, batos::common::int32_t y, batos::common::uint8_t colorIndex);
        };
    }
};

#endif