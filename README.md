# BatOS

This is a simple operating system with the following functionality:

- Ports that abstract communication to CPU using `IN/OUT` assembly instructions
- A Global Descriptor Table (GDT) containing information about memory segments for the CPU
- Interrupt management using Programmable Interrupt Controller (PIC) to handle inputs coming from external devices (keyboard, mouse)
- Custom drivers for keyboard, mouse and VGA
- A Peripheral Component Interconnect (PCI) to connect devices such as network, sound and graphics cards from manufacturers to the OS
- Dynamic memory management
- GUI functionality using VGA with a simple desktop and windows implementation

A more in depth description of how these features were implemented is written in the `documentation.pdf` file. Check it out!
