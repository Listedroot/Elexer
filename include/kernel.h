#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "interrupts.h"

// Kernel version
#define KERNEL_NAME    "ElexerKernel"
#define KERNEL_VERSION "0.1.0"

// Include VGA color definitions
#include "../drivers/vga.h"

// Serial port definitions
#define SERIAL_COM1_BASE 0x3F8

// IRQ numbers
#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

// Kernel panic handler
void panic(const char* message);

// Kernel entry point (defined in kernel.c)
__attribute__((noreturn)) extern void _kernel_main(void);

// VGA functions
void vga_clear(void);
void vga_set_color(uint8_t fg, uint8_t bg);
void vga_puts(const char* str);
void vga_putc(char c);
void vga_put_hex(uint32_t num);
void vga_set_cursor_visible(bool visible);

// Serial port functions
void serial_init(uint16_t port, uint32_t baud_rate);
void serial_write_byte(uint16_t port, char c);
void serial_write_string(uint16_t port, const char* str);

// Timer functions
void timer_init(uint32_t frequency);

// Keyboard functions
void keyboard_init(void);
char keyboard_get_char(void);

// Interrupt functions
void idt_init(void);
void register_interrupt_handler(uint8_t n, isr_t handler);

// Utility functions
void halt(void);
void cli(void);
void sti(void);

// Memory management
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif // KERNEL_H
