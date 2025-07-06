#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

// Initialize serial port
void serial_init(uint16_t port, uint32_t baud_rate);

// Send a single character to the serial port
void serial_write_char(uint16_t port, char c);

// Send a null-terminated string to the serial port
void serial_write_string(uint16_t port, const char* str);

// Check if data is available to read
uint8_t serial_received(uint16_t port);

// Read a single character from the serial port
char serial_read_char(uint16_t port);

// Read a line from the serial port (blocking)
void serial_read_line(uint16_t port, char* buffer, uint32_t max_length);

// Convenience macros for COM1
#define SERIAL1_WRITE_CHAR(c) serial_write_char(SERIAL_COM1_BASE, c)
#define SERIAL1_WRITE_STR(s) serial_write_string(SERIAL_COM1_BASE, s)

#endif // SERIAL_H
