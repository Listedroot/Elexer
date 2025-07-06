#include "../include/kernel.h"
#include "../drivers/vga.h"
#include "../drivers/serial.h"

// Kernel panic function - displays error message and halts the system
void panic(const char* message) {
    // Disable interrupts
    cli();
    
    // Set error color (white on red)
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_clear();
    
    // Print panic message to VGA
    vga_puts("\n*** KERNEL PANIC ***\n\n");
    vga_puts(message);
    
    // Also print to serial for debugging
    serial_write_string(SERIAL_COM1_BASE, "\n*** KERNEL PANIC ***\n\n");
    serial_write_string(SERIAL_COM1_BASE, message);
    serial_write_string(SERIAL_COM1_BASE, "\n");
    
    // Halt the CPU
    for (;;) {
        asm volatile ("hlt");
    }
}
