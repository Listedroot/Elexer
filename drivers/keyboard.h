#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include "../include/interrupts.h"

// Initialize keyboard driver and interrupt handler
void keyboard_init(void);

// Keyboard interrupt handler (called from IRQ1)
void keyboard_handler(registers_t *regs);

// Read one character from keyboard buffer (blocking)
char keyboard_get_char(void);

// Check if character is available (non-blocking)
int keyboard_has_char(void);

#endif
