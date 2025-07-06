#ifndef CONFIG_H
#define CONFIG_H

// Kernel configuration options

// Memory management
#define KERNEL_HEAP_SIZE 0x100000  // 1MB heap for kernel
#define PAGE_SIZE        4096      // 4KB pages

// VGA settings
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

// Serial port settings
#define SERIAL_COM1_BASE 0x3F8
#define SERIAL_BAUD_RATE 115200

// Interrupts
#define PIC1_COMMAND  0x20
#define PIC1_DATA     0x21
#define PIC2_COMMAND  0xA0
#define PIC2_DATA     0xA1

// Keyboard
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CMD_PORT  0x64

// Timer
#define PIT_FREQUENCY 100  // 100Hz

#endif // CONFIG_H
