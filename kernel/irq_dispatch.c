#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../include/kernel.h"
#include "../include/interrupts.h"
#include "../drivers/timer.h"
#include "../drivers/serial.h"
#include "../drivers/keyboard.h"

// Initialize the IRQ subsystem
void irq_init(void) {
    // Register our interrupt handlers
    register_interrupt_handler(IRQ0, timer_handler);
    register_interrupt_handler(IRQ1, keyboard_handler);
    
    // Initialize hardware that generates IRQs
    timer_init(100);  // 100Hz timer
    keyboard_init();
    
    // Enable interrupts
    sti();
}

// Default interrupt handler for unhandled IRQs
void default_irq_handler(registers_t *regs) {
    // Log the unhandled IRQ
    serial_write_string(SERIAL_COM1_BASE, "Unhandled IRQ: ");
    serial_write_byte(SERIAL_COM1_BASE, '0' + ((regs->int_no - 32) / 10));
    serial_write_byte(SERIAL_COM1_BASE, '0' + ((regs->int_no - 32) % 10));
    serial_write_string(SERIAL_COM1_BASE, "\n");
    
    // Acknowledge the interrupt
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20);  // Send EOI to slave
    }
    outb(0x20, 0x20);      // Send EOI to master
}
