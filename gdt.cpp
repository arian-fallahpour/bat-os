#include "gdt.h"

GlobalDescriptorTable::GlobalDescriptorTable()
    : nullSegmentSelector(0, 0, 0),
      unusedSegmentSelector(0, 0, 0),
      codeSegmentSelector(0, 64 * 1024 * 1024,  // 64 MB
                          0x9A),                // executable, readable, accessed
      dataSegmentSelector(0, 64 * 1024 * 1024,  // 64 MB
                          0x92)                 // writable, accessed
{
  uint32_t i[2];
  i[0] = (uint32_t)this;
  i[1] = sizeof(GlobalDescriptorTable) << 16;

  asm volatile("lgdt (%0)" : : "p"(((uint8_t*)i) + 2));
};

GlobalDescriptorTable::~GlobalDescriptorTable() {};

uint16_t GlobalDescriptorTable::dataSegmentSelector() {
  return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
};

uint16_t GlobalDescriptorTable::codeSegmentSelector() {
  return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
};

GlobalDescriptorTable::SegmentSelector::SegmentDescriptor(uint32_t base, uint32_t limit,
                                                          uint8_t flags) {
  uint8_t* target = (uint8_t*)this;

  if (limit <= 65536) {
    target[6] = 0x40  // 16-bit segment
  } else {
    if ((limit & 0xFFF) != 0xFFF) {
      limit = (limit >> 12) - 1;
    } else {
      limit = limit >> 12;
    }
    target[6] = 0xC0;
  }

  target[0] = limit & 0xFF;
  target[1] = (limit >> 8) & 0xFF;
  target[6] |= (limit >> 16) & 0xF;

  target[2] = base & 0xFF;
  target[3] = (base >> 8) & 0xFF;
  target[4] = (base >> 16) & 0xFF;
  target[7] = (base >> 24) & 0xFF;

  target[5] = flags;
}