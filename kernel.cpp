#include "types.h"

void print(char* str) {
    const uint8_t widthLimit = 80; // Screen width is 80 characters on old OSes
    const uint8_t heightLimit = 25; // Screen height is 25 lines on old OSes
    static uint8_t x = 0, y = 0;

    static uint16_t* VideoMemory = (uint16_t*)0xb8000; // Video memory begins at address 0xb8000

    for (int i = 0; str[i] != '\0'; i++) {
        switch(str[i]) {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[widthLimit*y + x] = (VideoMemory[widthLimit*y + x] & 0xFF00) | str[i]; // OxFF00 black background, white text
                x++;
        }

        // If we reach the end of the line, go to the next line
        if (x >= widthLimit) {
            x = 0;
            y++;
        }

        // If we reach the bottom of the screen, clear it and start from the top
        if (y >= heightLimit) {
            for (y = 0; y < heightLimit; y++) {
                for (x = 0; x < widthLimit; x++) {
                    VideoMemory[widthLimit*y + x] = (VideoMemory[widthLimit*y + x] & 0xFF00) | ' ';
                }
            }
            
            x = 0;
            y = 0;
        }
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors; 
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for (constructor* i = &start_ctors; i != &end_ctors; i++) {
        (*i)();
    }
} 

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnumber) {
    print("Hello World!\nHello World!");  
    // print("Hello World!\n");  

    while(1);
};