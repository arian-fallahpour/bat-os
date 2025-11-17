#ifndef __BATOS__GDT_H
#define __BATOS__GDT_H

#include <common/types.h>

namespace batos {
    class GlobalDescriptorTable {
        public:
            class SegmentDescriptor {
                private:
                    batos::common::uint16_t limit_lo;
                    batos::common::uint16_t base_lo;
                    batos::common::uint8_t base_hi;
                    batos::common::uint8_t type;
                    batos::common::uint8_t flags_limit_hi;
                    batos::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(
                        batos::common::uint32_t base,
                        batos::common::uint32_t limit,
                        batos::common::uint8_t type
                    );

                    batos::common::uint32_t Base();
                    batos::common::uint32_t Limit();

            } __attribute__((packed));

            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:
            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            batos::common::uint16_t CodeSegmentSelector();
            batos::common::uint16_t DataSegmentSelector();
    };
};  // namespace batos

#endif