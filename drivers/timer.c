#include "timer.h"
#include "vga.h"   // optional debug output
#include <stdint.h>
#include "../include/kernel/io.h"

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182

static volatile uint32_t ticks = 0;

// inb and outb are defined in kernel/io.h

// Initialize PIT to generate interrupts at frequency Hz
void timer_init(uint32_t frequency) {
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);
    outb(PIT_COMMAND, 0x36);             // channel 0, lobyte/hibyte, mode 3 (square wave)
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)(divisor >> 8));
    // Enable IRQ0 on PIC (assumes PIC remapping done elsewhere)
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 0); // clear mask for IRQ0
    outb(0x21, mask);
}

void timer_handler(registers_t *regs) {
    (void)regs; // Mark as unused to prevent warning
    ticks++;
    
    // Acknowledge the timer interrupt
    outb(0x20, 0x20);
    
    // Optional: do something every tick
}

uint32_t timer_get_ticks(void) {
    return ticks;
}
