#include "../include/kernel.h"
#include "../drivers/vga.h"
#include "../drivers/serial.h"

// Simple kernel print function that outputs to both VGA and serial
void kprint(const char* str) {
    // Output to VGA
    vga_puts(str);
    
    // Output to serial (COM1) for debugging
    for (size_t i = 0; str[i] != '\0'; i++) {
        serial_write_byte(SERIAL_COM1_BASE, str[i]);
    }
}

// Print a single character
void kputc(char c) {
    vga_putc(c);
    serial_write_byte(SERIAL_COM1_BASE, c);
}

// Clear screen
void kclear(void) {
    vga_clear();
}
