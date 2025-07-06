#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Read a byte from the specified port
static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

// Write a byte to the specified port
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Read a word (16 bits) from the specified port
static inline uint16_t inw(uint16_t port) {
    uint16_t val;
    __asm__ volatile("inw %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

// Write a word (16 bits) to the specified port
static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Read a double word (32 bits) from the specified port
static inline uint32_t inl(uint16_t port) {
    uint32_t val;
    __asm__ volatile("inl %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

// Write a double word (32 bits) to the specified port
static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

// I/O wait - used to create a small delay
static inline void io_wait(void) {
    // Write to an unused port to create a small delay
    outb(0x80, 0);
}


// Halt the CPU until the next interrupt
static inline void hlt(void) {
    __asm__ volatile("hlt");
}

// Read the EFLAGS register
static inline uint32_t read_eflags(void) {
    uint32_t eflags;
    __asm__ volatile("pushf; pop %0" : "=r"(eflags));
    return eflags;
}

// Write to the EFLAGS register
static inline void write_eflags(uint32_t eflags) {
    __asm__ volatile("push %0; popf" : : "r"(eflags));
}

// Check if interrupts are enabled
static inline bool are_interrupts_enabled(void) {
    return (read_eflags() & (1 << 9)) != 0;
}

#endif // IO_H
