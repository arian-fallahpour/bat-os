#include "mouse.h"

MouseDriver::MouseDriver(InterruptManager* manager): 
    InterruptHandler(0x2C, manager), 
    dataport(0x60),
    commandport(0x64) {}

MouseDriver::~MouseDriver() {

}

void printf(char* str);

void MouseDriver::Activate() {
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    offset = 0;
    buttons = 0;
    x = 40;
    y = 12;
    VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
                            | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
                            | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB

    commandport.Write(0xA8); // Start sending mouse interrupts
    commandport.Write(0x20); // Command 0x20 = read controller command byte
    uint8_t status = dataport.Read() | 2;
    commandport.Write(0x60); // Change state
    dataport.Write(status);

    commandport.Write(0xD4); // Tell the mouse we are sending a command
    dataport.Write(0xF4); // Activate mouse
    dataport.Read();
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = commandport.Read();
    if (!(status & 0x20)) { // Test if there is data
        return esp;
    }

    static int8_t x = 40, y = 12;

    buffer[offset] = dataport.Read();   
    offset = (offset + 1) % 3;

    // Mouse movement calculation
    if (offset == 0) {
        if (buffer[1] != 0 || buffer[2] != 0) {

            static uint16_t* VideoMemory = (uint16_t*)0xb8000;
            VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
                                    | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
                                    | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB
            
            x += (int8_t)buffer[1] / 2;
            if (x < 0) x = 0;
            if (x >= 80) x = 79;
            
            y -= (int8_t)buffer[2] / 2;
            if (y < 0) y = 0;
            if (y >= 25) y = 24;
            
            VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
                                    | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
                                    | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB
        }

        // for (uint8_t i = 0; i < 3; i++) {
        //     if ((buffer[0] && (0x01 << i)) != (buttons && (0x01 << i))) {
        //         VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
        //                                 | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
        //                                 | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB
        //     }
        // }
        // buttons = buffer[0];
    }

    return esp;
}