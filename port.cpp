#include "port.h"

Port::Port(uint16_t portNumber) {
    this->portNumber = portNumber;
};

Port::~Port() {
};

Port8Bit::Port8Bit(uint16_t portNumber) : Port(portNumber) {
};

Port8Bit::~Port8Bit() {
};

// PORT8BIT
void Port8Bit::Write(uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portNumber));
};

uint8_t Port8Bit::Read() {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
};

// PORT8BITSLOW
Port8BitSlow::Port8BitSlow(uint16_t portNumber) : Port8Bit(portNumber) {
};

Port8BitSlow::~Port8BitSlow() {
};

void Port8BitSlow::Write(uint8_t data) {
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portNumber));
};

// PORT16BIT
Port16Bit::Port16Bit(uint16_t portNumber) : Port(portNumber) {
};

Port16Bit::~Port16Bit() {
};

void Port16Bit::Write(uint16_t data) {
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portNumber));
};

uint16_t Port16Bit::Read() {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
};

// PORT32BIT
Port32Bit::Port32Bit(uint32_t portNumber) : Port(portNumber) {
};

Port32Bit::~Port32Bit() {
};

void Port32Bit::Write(uint32_t data) {
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portNumber));
};

uint32_t Port32Bit::Read() {
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
};

