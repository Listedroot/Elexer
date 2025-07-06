#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include "../include/kernel/io.h"

// COM1 base port
#define SERIAL_COM1_BASE 0x3F8

// Initialize COM1 serial port at 115200 baud, 8N1
void serial_init(uint16_t port, uint32_t baud_rate);

// Write a single byte to serial port (blocking)
void serial_write_byte(uint16_t port, char c);

// Write a null-terminated string to serial port
void serial_write_string(uint16_t port, const char* str);

// Check if data is available to read
uint8_t serial_received(uint16_t port);

// Read a single character from the serial port
char serial_read_char(uint16_t port);

// Read a line from the serial port (blocking)
void serial_read_line(uint16_t port, char* buffer, uint32_t max_length);

// Write a hexadecimal value to the serial port
void serial_write_hex(uint16_t port, uint32_t n);

// Convenience macros for COM1
#define SERIAL1_WRITE_CHAR(c) serial_write_byte(SERIAL_COM1_BASE, c)
#define SERIAL1_WRITE_STR(s) serial_write_string(SERIAL_COM1_BASE, s)

// Legacy functions for backward compatibility
static inline void serial_init_simple(void) {
    serial_init(SERIAL_COM1_BASE, 115200);
}

static inline void serial_write(const char* str) {
    serial_write_string(SERIAL_COM1_BASE, str);
}

static inline int serial_is_transmit_empty(void) {
    return (inb(SERIAL_COM1_BASE + 5) & 0x20) != 0;
}

#endif // SERIAL_H