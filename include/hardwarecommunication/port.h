#ifndef __BATOS__HARDWARECOMMUNICATION__PORT_H
#define __BATOS__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace batos {
    namespace hardwarecommunication {
        class Port {
            protected:
                batos::common::uint16_t portNumber;
                Port(batos::common::uint16_t portNumber);
                ~Port();
        };

        class Port8Bit : public Port {
            public:
                Port8Bit(batos::common::uint16_t portNumber);
                ~Port8Bit();
                virtual void Write(batos::common::uint8_t data);
                virtual batos::common::uint8_t Read();
        };

        class Port8BitSlow : public Port8Bit {
            public:
                Port8BitSlow(batos::common::uint16_t portNumber);
                ~Port8BitSlow();
                virtual void Write(batos::common::uint8_t data);
        };

        class Port16Bit : public Port {
            public:
                Port16Bit(batos::common::uint16_t portNumber);
                ~Port16Bit();
                virtual void Write(batos::common::uint16_t data);
                virtual batos::common::uint16_t Read();
        };

        class Port32Bit : public batos::hardwarecommunication::Port {
            public:
                Port32Bit(batos::common::uint32_t portNumber);
                ~Port32Bit();
                virtual void Write(batos::common::uint32_t data);
                virtual batos::common::uint32_t Read();
        };
    }  // namespace hardwarecommunication
};  // namespace batos

#endif