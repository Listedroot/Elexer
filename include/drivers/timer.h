#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

// Timer ports
#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_CMD 0x43

// Timer modes
#define PIT_MODE0 0x00  // Interrupt on terminal count
#define PIT_MODE2 0x04  // Rate generator
#define PIT_MODE3 0x06  // Square wave generator

// Access modes
#define PIT_ACC_LATCH 0x00
#define PIT_ACC_LO 0x10
#define PIT_ACC_HI 0x20
#define PIT_ACC_LOHI 0x30

// Channel selection
#define PIT_CHANNEL0_SEL 0x00
#define PIT_CHANNEL1_SEL 0x40
#define PIT_CHANNEL2_SEL 0x80
#define PIT_READBACK 0xC0

// BCD mode
#define PIT_BINARY 0x00
#define PIT_BCD 0x01

// Timer frequency (1.193182 MHz)
#define PIT_FREQUENCY 1193180

// Initialize the PIT (Programmable Interval Timer)
// frequency: Desired timer frequency in Hz (18.2065 Hz to 1.1931 MHz)
void timer_init(uint32_t frequency);

// Get the current tick count
uint32_t timer_get_ticks(void);

// Get the current time in milliseconds
uint64_t timer_get_ms(void);

// Get the current time in seconds
uint64_t timer_get_seconds(void);

// Wait for a specified number of milliseconds
void timer_wait(uint32_t milliseconds);

// Register a timer callback function
// Returns a timer ID that can be used to unregister the callback
int timer_register_callback(void (*callback)(void), uint32_t interval_ms, bool repeat);

// Unregister a timer callback
void timer_unregister_callback(int timer_id);

// Sleep for the specified number of milliseconds
void sleep(uint32_t milliseconds);

#endif // TIMER_H
