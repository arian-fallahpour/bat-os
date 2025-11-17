#include <drivers/keyboard.h>

using namespace batos::common;
using namespace batos::drivers;
using namespace batos::hardwarecommunication;

KeyboardEventHandler::KeyboardEventHandler() {};
void KeyboardEventHandler::OnKeyDown(char c) {}
void KeyboardEventHandler::OnKeyUp(char c) {}

KeyboardDriver::KeyboardDriver(
    InterruptManager* manager,
    KeyboardEventHandler* handler
)
    : InterruptHandler(0x21, manager), dataport(0x60), commandport(0x64) {
    this->handler = handler;
}

KeyboardDriver::~KeyboardDriver() {}

void printf(char* str);
void printfHex(uint8_t key);

void KeyboardDriver::Activate() {
    while (commandport.Read() & 0x1) {
        dataport.Read();
    }
    commandport.Write(0xAE);  // Start sending keyboards interrupts
    commandport.Write(0x20);  // Command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60);  // Change state
    dataport.Write(status);

    dataport.Write(0xF4);  // Activate keyboard
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    uint8_t key = dataport.Read();
    if (handler == 0) {
        return esp;
    }

    static bool Shift = false;

    // Note: On different keyboards/scancodesets, these values may differ
    // Key pressed
    switch (key) {
        case 0xFA:
            break;  // ACK

            // clang-format off
        case 0x02: handler->OnKeyDown(!Shift ? '1' : '!'); break;
        case 0x03: handler->OnKeyDown(!Shift ? '2' : '@'); break;
        case 0x04: handler->OnKeyDown(!Shift ? '3' : '#'); break;
        case 0x05: handler->OnKeyDown(!Shift ? '4' : '$'); break;
        case 0x06: handler->OnKeyDown(!Shift ? '5' : '%'); break;
        case 0x07: handler->OnKeyDown(!Shift ? '6' : '^'); break;
        case 0x08: handler->OnKeyDown(!Shift ? '7' : '&'); break;
        case 0x09: handler->OnKeyDown(!Shift ? '8' : '*'); break;
        case 0x0A: handler->OnKeyDown(!Shift ? '9' : '('); break;
        case 0x0B: handler->OnKeyDown(!Shift ? '0' : ')'); break;
        case 0x0C: handler->OnKeyDown(!Shift ? '-' : '_'); break;
        case 0x0D: handler->OnKeyDown(!Shift ? '=' : '+'); break;
        case 0x27: handler->OnKeyDown(!Shift ? ';' : ':'); break;
        case 0x28: handler->OnKeyDown(!Shift ? '\'' : '\"'); break;
        case 0x1A: handler->OnKeyDown(!Shift ? '[' : '{'); break;
        case 0x1B: handler->OnKeyDown(!Shift ? ']' : '}'); break;
        case 0x2B: handler->OnKeyDown(!Shift ? '\\' : '|'); break;
        case 0x29: handler->OnKeyDown(!Shift ? '`' : '~'); break;
        case 0x33: handler->OnKeyDown(!Shift ? ',' : '<'); break;
        case 0x34: handler->OnKeyDown(!Shift ? '.' : '>'); break;
        case 0x35: handler->OnKeyDown(!Shift ? '/' : '?'); break;

        case 0x10: handler->OnKeyDown(!Shift ? 'q' : 'Q'); break;
        case 0x11: handler->OnKeyDown(!Shift ? 'w' : 'W'); break;
        case 0x12: handler->OnKeyDown(!Shift ? 'e' : 'E'); break;
        case 0x13: handler->OnKeyDown(!Shift ? 'r' : 'R'); break;
        case 0x14: handler->OnKeyDown(!Shift ? 't' : 'T'); break;
        case 0x15: handler->OnKeyDown(!Shift ? 'y' : 'Y'); break;
        case 0x16: handler->OnKeyDown(!Shift ? 'u' : 'U'); break;
        case 0x17: handler->OnKeyDown(!Shift ? 'i' : 'I'); break;
        case 0x18: handler->OnKeyDown(!Shift ? 'o' : 'O'); break;
        case 0x19: handler->OnKeyDown(!Shift ? 'p' : 'P'); break;
        case 0x1E: handler->OnKeyDown(!Shift ? 'a' : 'A'); break;
        case 0x1F: handler->OnKeyDown(!Shift ? 's' : 'S'); break;
        case 0x20: handler->OnKeyDown(!Shift ? 'd' : 'D'); break;
        case 0x21: handler->OnKeyDown(!Shift ? 'f' : 'F'); break;
        case 0x22: handler->OnKeyDown(!Shift ? 'g' : 'G'); break;
        case 0x23: handler->OnKeyDown(!Shift ? 'h' : 'H'); break;
        case 0x24: handler->OnKeyDown(!Shift ? 'j' : 'J'); break;
        case 0x25: handler->OnKeyDown(!Shift ? 'k' : 'K'); break;
        case 0x26: handler->OnKeyDown(!Shift ? 'l' : 'L'); break;
        case 0x2C: handler->OnKeyDown(!Shift ? 'z' : 'Z'); break;
        case 0x2D: handler->OnKeyDown(!Shift ? 'x' : 'X'); break;
        case 0x2E: handler->OnKeyDown(!Shift ? 'c' : 'C'); break;
        case 0x2F: handler->OnKeyDown(!Shift ? 'v' : 'V'); break;
        case 0x30: handler->OnKeyDown(!Shift ? 'b' : 'B'); break;
        case 0x31: handler->OnKeyDown(!Shift ? 'n' : 'N'); break;
        case 0x32: handler->OnKeyDown(!Shift ? 'm' : 'M'); break;

        case 0x39: handler->OnKeyDown(' '); break; // Space
        case 0x1C: handler->OnKeyDown('\n'); break; // Enter
        case 0x2A: case 0x36: Shift = true; break; // Shift pressed
        case 0xAA: case 0xB6: Shift = false; break; // Shift released

        case 0x45: // Num Lock
        case 0xC5: break;

        // clang-format on
        default:
            if (key < 0x80) {
                printf("KEYBOARD 0x");
                printfHex(key);
            }
            break;
    }

    return esp;
}