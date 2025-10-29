#include "Keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager): 
    InterruptHandler(0x21, manager), 
    dataport(0x60),
    commandport(0x64) {
    
}

KeyboardDriver::~KeyboardDriver() {

}

void printf(char* str);
void printfHex(uint8_t key);

void KeyboardDriver::Activate() {
    while (commandport.Read() & 0x1) {
        dataport.Read();
    }
    commandport.Write(0xAE); // Start sending keyboards interrupts
    commandport.Write(0x20); // Command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // Change state
    dataport.Write(status);

    dataport.Write(0xF4); // Activate keyboard
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    uint8_t key = dataport.Read();

    static bool Shift = false;

    // Note: On different keyboards/scancodesets, these values may differ
    // Key pressed   
    switch (key) {
        case 0xFA: break; // ACK

        case 0x02: if (!Shift) printf("1"); else printf("!"); break;
        case 0x03: if (!Shift) printf("2"); else printf("@"); break;
        case 0x04: if (!Shift) printf("3"); else printf("#"); break;
        case 0x05: if (!Shift) printf("4"); else printf("$"); break;
        case 0x06: if (!Shift) printf("5"); else printf("%"); break;
        case 0x07: if (!Shift) printf("6"); else printf("^"); break;
        case 0x08: if (!Shift) printf("7"); else printf("&"); break;
        case 0x09: if (!Shift) printf("8"); else printf("*"); break;
        case 0x0A: if (!Shift) printf("9"); else printf("("); break;
        case 0x0B: if (!Shift) printf("0"); else printf(")"); break;
        case 0x0C: if (!Shift) printf("-"); else printf("_"); break;
        case 0x0D: if (!Shift) printf("="); else printf("+"); break;
        case 0x27: if (!Shift) printf(";"); else printf(":"); break;
        case 0x28: if (!Shift) printf("'"); else printf("\""); break;
        case 0x1A: if (!Shift) printf("["); else printf("{"); break;
        case 0x1B: if (!Shift) printf("]"); else printf("}"); break;
        case 0x2B: if (!Shift) printf("\\"); else printf("|"); break;
        case 0x29: if (!Shift) printf("`"); else printf("~"); break;
        case 0x33: if (!Shift) printf(","); else printf("<"); break;
        case 0x34: if (!Shift) printf("."); else printf(">"); break;
        case 0x35: if (!Shift) printf("/"); else printf("?"); break;

        case 0x10: if (!Shift) printf("q"); else printf("Q"); break;
        case 0x11: if (!Shift) printf("w"); else printf("W"); break;
        case 0x12: if (!Shift) printf("e"); else printf("E"); break;
        case 0x13: if (!Shift) printf("r"); else printf("R"); break;
        case 0x14: if (!Shift) printf("t"); else printf("T"); break;
        case 0x15: if (!Shift) printf("y"); else printf("Y"); break;
        case 0x16: if (!Shift) printf("u"); else printf("U"); break;
        case 0x17: if (!Shift) printf("i"); else printf("I"); break;
        case 0x18: if (!Shift) printf("o"); else printf("O"); break;
        case 0x19: if (!Shift) printf("p"); else printf("P"); break;
        case 0x1E: if (!Shift) printf("a"); else printf("A"); break;
        case 0x1F: if (!Shift) printf("s"); else printf("S"); break;
        case 0x20: if (!Shift) printf("d"); else printf("D"); break;
        case 0x21: if (!Shift) printf("f"); else printf("F"); break;
        case 0x22: if (!Shift) printf("g"); else printf("G"); break;
        case 0x23: if (!Shift) printf("h"); else printf("H"); break;
        case 0x24: if (!Shift) printf("j"); else printf("J"); break;
        case 0x25: if (!Shift) printf("k"); else printf("K"); break;
        case 0x26: if (!Shift) printf("l"); else printf("L"); break;
        case 0x2C: if (!Shift) printf("z"); else printf("Z"); break;
        case 0x2D: if (!Shift) printf("x"); else printf("X"); break;
        case 0x2E: if (!Shift) printf("c"); else printf("C"); break;
        case 0x2F: if (!Shift) printf("v"); else printf("V"); break;
        case 0x30: if (!Shift) printf("b"); else printf("B"); break;
        case 0x31: if (!Shift) printf("n"); else printf("N"); break;
        case 0x32: if (!Shift) printf("m"); else printf("M"); break;

        case 0x39: printf(" "); break; // Space
        case 0x1C: printf("\n"); break; // Enter
        case 0x2A: case 0x36: Shift = true; break; // Shift pressed
        case 0xAA: case 0xB6: Shift = false; break; // Shift released

        case 0x45: // Num Lock
        case 0xC5: break;
        
        default:
            if (key < 0x80) {
                printf("KEYBOARD 0x");
                printfHex(key);
            }
            break;
    }

    return esp;
}