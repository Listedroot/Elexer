#include "serial.h"
#include <stdint.h>
#include "../include/kernel/io.h"

void serial_init(uint16_t port, uint32_t baud_rate) {
    uint16_t divisor = 115200 / baud_rate;
    
    // Disable all interrupts
    outb(port + 1, 0x00);
    
    // Enable DLAB (set baud rate divisor)
    outb(port + 3, 0x80);
    
    // Set divisor (lo byte/hi byte)
    outb(port + 0, (divisor >> 0) & 0xFF);
    outb(port + 1, (divisor >> 8) & 0xFF);
    
    // 8 bits, no parity, one stop bit
    outb(port + 3, 0x03);
    
    // Enable FIFO, clear them, 14-byte threshold
    outb(port + 2, 0xC7);
    
    // IRQs enabled, RTS/DSR set
    outb(port + 4, 0x0B);
}

void serial_write_byte(uint16_t port, char c) {
    while ((inb(port + 5) & 0x20) == 0) {
        __asm__ volatile("pause");
    }
    outb(port, c);
}

void serial_write_string(uint16_t port, const char* str) {
    while (*str) {
        if (*str == '\n') {
            serial_write_byte(port, '\r');
        }
        serial_write_byte(port, *str++);
    }
}

uint8_t serial_received(uint16_t port) {
    return inb(port + 5) & 1;
}

char serial_read_char(uint16_t port) {
    while (!serial_received(port)) {
        __asm__ volatile("pause");
    }
    return inb(port);
}

void serial_write_hex(uint16_t port, uint32_t n) {
    const char* hex_chars = "0123456789ABCDEF";
    char buffer[9]; // 8 hex digits + null terminator
    
    // Convert each nibble to a hex character
    for (int i = 7; i >= 0; i--) {
        buffer[7 - i] = hex_chars[(n >> (i * 4)) & 0xF];
    }
    buffer[8] = '\0';
    
    // Skip leading zeros
    int start = 0;
    while (start < 7 && buffer[start] == '0') {
        start++;
    }
    
    // Write the hex string
    serial_write_string(port, "0x");
    serial_write_string(port, buffer + start);
}

void serial_read_line(uint16_t port, char* buffer, uint32_t max_length) {
    uint32_t i = 0;
    char c;
    
    while (i < max_length - 1) {
        c = serial_read_char(port);
        
        // Echo the character back
        serial_write_byte(port, c);
        
        // Handle backspace
        if (c == '\b' && i > 0) {
            i--;
            buffer[i] = '\0';
            continue;
        }
        
        // Handle newline/enter
        if (c == '\r' || c == '\n') {
            buffer[i] = '\0';
            serial_write_byte(port, '\n');
            return;
        }
        
        // Add character to buffer
        buffer[i++] = c;
    }
    
    // Null-terminate the string
    buffer[i] = '\0';
}
