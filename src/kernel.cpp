#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h> 

// #define GRAPHICSMODE

using namespace batos;
using namespace batos::common;
using namespace batos::drivers;
using namespace batos::hardwarecommunication;
using namespace batos::gui;
using namespace batos::multitasking;

void printf(char* str) {
  const uint8_t widthLimit = 80;   // Screen width is 80 characters on old OSes
  const uint8_t heightLimit = 25;  // Screen height is 25 lines on old OSes
  static uint8_t x = 0, y = 0;

  static uint16_t* VideoMemory = (uint16_t*)0xb8000;  // Video memory begins at address 0xb8000

  for (int i = 0; str[i] != '\0'; i++) {
    switch (str[i]) {
      case '\n':
        x = 0;
        y++;
        break;
      default:
        VideoMemory[widthLimit * y + x] =
            (VideoMemory[widthLimit * y + x] & 0xFF00) |
            str[i];  // OxFF00 black background, white text
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
          VideoMemory[widthLimit * y + x] =
              (VideoMemory[widthLimit * y + x] & 0xFF00) | ' ';
        }
      }

      x = 0;
      y = 0;
    }
  }
}

void printfHex(uint8_t key) {
  char* foo = "00";
  char* hex = "0123456789ABCDEF";
  foo[0] = hex[(key >> 4) & 0x0F];
  foo[1] = hex[key & 0x0F];
  printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler {
  public:
    void OnKeyDown(char c) {
      char* foo = " ";
      foo[0] = c;
      printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler {
  int8_t x, y;

  public:
    MouseToConsole() {
      uint16_t* VideoMemory = (uint16_t*)0xb8000;
      x = 40;
      y = 12;
      VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
                              | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
                              | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB
    }

    void OnMouseMove(int8_t xoffset, int8_t yoffset) {
      static uint16_t* VideoMemory = (uint16_t*)0xb8000;
      VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
                              | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
                              | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB
      
      x += (int8_t)xoffset / 2;
      if (x < 0) x = 0;
      if (x >= 80) x = 79;
      
      y += (int8_t)yoffset / 2;
      if (y < 0) y = 0;
      if (y >= 25) y = 24;
      
      VideoMemory[80 * y + x] = ((VideoMemory[80 * y + x] & 0xF000) >> 4) // Move bits at F 4 to right
                              | ((VideoMemory[80 * y + x] & 0x0F00) << 4) // Move bits at F 4 to left
                              | ((VideoMemory[80 * y + x] & 0x00FF)); // Draw mouse cursor by flipping character at (x, y) to 0xDB
    }
};

void taskA() {
  while (true) {
    printf("A");
  }
}

void taskB() {
  while (true) {
    printf("B");
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
  printf("Hello World!\nHello World!");
  
  GlobalDescriptorTable gdt;
  
  TaskManager taskManager;
  Task task1(&gdt, taskA);
  Task task2(&gdt, taskB);
  // taskManager.AddTask(&task1);
  // taskManager.AddTask(&task2);

  InterruptManager interruptManager(0x20, &gdt, &taskManager);
  
  printf("Initializing Hardware, Stage 1\n");

#ifdef GRAPHICSMODE
  Desktop desktop(320, 200, 0x00, 0x00, 0xA8);
#endif

  DriverManager drvManager;
  
#ifdef GRAPHICSMODE
  KeyboardDriver keyboard(&interruptManager, &desktop); // attach keyboard to desktop
#else
  PrintfKeyboardEventHandler kbhandler;
  KeyboardDriver keyboard(&interruptManager, &kbhandler);
#endif
  drvManager.addDriver(&keyboard);


#ifdef GRAPHICSMODE
  MouseDriver mouse(&interruptManager, &desktop); // attach mouse to desktop
#else
  MouseToConsole mousehandler;
  MouseDriver mouse(&interruptManager, &mousehandler);
#endif
  drvManager.addDriver(&mouse);

  PeripheralComponentInterconnectController PCIController;
  PCIController.SelectDrivers(&drvManager, &interruptManager);

#ifdef GRAPHICSMODE
  VideoGraphicsArray vga;
#endif
  
  printf("Initializing Hardware, Stage 2\n");
  drvManager.ActivateAll();
  
  printf("Initializing Hardware, Stage 3\n");

#ifdef GRAPHICSMODE
  vga.SetMode(320, 200, 8);

  Window win1(&desktop, 10, 10, 20, 20, 0xA8, 0x00, 0x00);
  desktop.AddChild(&win1);
  Window win2(&desktop, 40, 15, 30, 30, 0x00, 0xA8, 0x00);
  desktop.AddChild(&win2);
#endif
  
  interruptManager.Activate();
  
  while (1) {
#ifdef GRAPHICSMODE
    desktop.Draw(&vga); // not a good idea bc of multitasking later
#endif
  }
};