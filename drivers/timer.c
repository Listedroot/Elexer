#include "timer.h"
#include "../drivers/vga.h"   // optional debug output
#include <stdint.h>

#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43
#define PIT_FREQUENCY 1193182

static volatile uint32_t ticks = 0;

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

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

void timer_handler(void) {
    ticks++;
    // Optional: do something every tick
}

uint32_t timer_get_ticks(void) {
    return ticks;
}
