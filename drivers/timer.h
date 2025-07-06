#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// Initialize PIT to given frequency (Hz)
void timer_init(uint32_t frequency);

// Called from IRQ0 (timer interrupt handler)
void timer_handler(void);

// Get system tick count (increments every timer interrupt)
uint32_t timer_get_ticks(void);

#endif
