#include "kernel.h"
#include "config.h"
#include "../drivers/vga.h"
#include "../drivers/serial.h"
#include "interrupts.h"
#include <stdint.h>

// Forward declaration
void irq_init(void);

// Simple minimal console
static void console_loop(void) {
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    while (1) {
        char c = keyboard_get_char();
        if (c) {
            if (c == 0x03) { // Ctrl-C triggers panic for demo
                panic("User triggered panic (Ctrl-C)");
            }
            serial_write_byte(SERIAL_COM1_BASE, c);
            vga_putc(c);
            
            // Echo newline on Enter key
            if (c == '\r') {
                vga_putc('\n');
            }
        }
    }
}

// Default interrupt handler
static void default_handler(registers_t *regs) {
    vga_puts("Unhandled interrupt: 0x");
    vga_puthex(regs->int_no);
    vga_puts(" (");
    
    // Print error code if present
    if (regs->err_code != 0xFFFFFFFF) {
        vga_puthex(regs->err_code);
    } else {
        vga_puts("no error code");
    }
    
    vga_puts(")\n");
}

void _kernel_main(void) {
    // Initialize VGA and clear screen
    vga_clear();
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    vga_puts("ElexerKernel v" KERNEL_VERSION "\n");
    vga_puts("Initializing subsystems...\n");

    // Initialize serial port for debugging
    serial_init(SERIAL_COM1_BASE, 115200);
    serial_write_string(SERIAL_COM1_BASE, "Serial port ready.\n");

    // Initialize IDT, PIC, and IRQ handling
    idt_init();
    irq_init();
    
    // Register default handler for unhandled interrupts
    for (int i = 0; i < 48; i++) {
        register_interrupt_handler(i, default_handler);
    }
    
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    vga_puts("Initialization complete!\n");
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    // Print prompt
    vga_puts("Type something (Ctrl-C to panic): ");
    
    // Enter console loop (this function should never return)
    console_loop();
    
    // If console_loop somehow returns, enter an infinite loop
    while (1) {
        __asm__ volatile ("hlt");
    }
    
    // This code should never be reached
    __builtin_unreachable();
}
