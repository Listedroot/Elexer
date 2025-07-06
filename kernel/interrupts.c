#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "../include/kernel/io.h"
#include "../include/kernel/pic.h"
#include "../include/interrupts.h"
#include "../drivers/serial.h"
#include "../include/kernel.h"  // For panic()

// Ensure NULL is defined if not already
#ifndef NULL
#define NULL ((void*)0)
#endif

// Define IDT (declared as extern in interrupts.h)
idt_entry_t idt[IDT_ENTRIES];

// ISR handler function pointers
static isr_t interrupt_handlers[IDT_ENTRIES];

// External declarations for assembly functions
typedef void (*isr_handler_t)(void);
extern isr_handler_t _isr_handlers[];
extern isr_handler_t _irq_handlers[];
extern void _idt_flush(uint32_t);

// Default interrupt handler
void default_interrupt_handler(registers_t *r) {
    // Don't panic for spurious IRQs or IRQs that might be handled by drivers
    if (r->int_no < 32) {
        char buf[32];
        const char *msg = "Unhandled exception ";
        char *ptr = buf;
        
        // Copy the message
        while (*msg) {
            *ptr++ = *msg++;
        }
        
        // Convert the number to string
        int num = r->int_no;
        char num_buf[16];
        char *num_ptr = num_buf;
        
        if (num == 0) {
            *num_ptr++ = '0';
        } else {
            // Convert number to string (reversed)
            while (num > 0) {
                *num_ptr++ = '0' + (num % 10);
                num /= 10;
            }
            // Reverse the number string
            char *start = num_buf;
            char *end = num_ptr - 1;
            while (start < end) {
                char tmp = *start;
                *start = *end;
                *end = tmp;
                start++;
                end--;
            }
        }
        *num_ptr = '\0';
        
        // Copy the number
        char *tmp = num_buf;
        while (*tmp) {
            *ptr++ = *tmp++;
        }
        *ptr = '\0';
        
        panic(buf);
    } else if (r->int_no >= 32 && r->int_no < 48) {
        // This is an IRQ, just log it
        serial_write_string(SERIAL_COM1_BASE, "Unhandled IRQ: ");
        serial_write_byte(SERIAL_COM1_BASE, '0' + ((r->int_no - 32) / 10));
        serial_write_byte(SERIAL_COM1_BASE, '0' + ((r->int_no - 32) % 10));
        serial_write_string(SERIAL_COM1_BASE, "\n");
    }
    
    // Acknowledge the interrupt
    if (r->int_no >= 40) {
        outb(0xA0, 0x20);  // Send EOI to slave
    }
    if (r->int_no >= 32) {
        outb(0x20, 0x20);  // Send EOI to master
    }
}

// Register an interrupt handler
void register_interrupt_handler(uint8_t n, isr_t handler) {
    // No need to check upper bound since n is uint8_t (0-255) and IDT_ENTRIES is 256
    // So we just need to make sure the handler is not NULL
    if (handler != NULL) {
        interrupt_handlers[n] = handler;
    }
}

// Set an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags | IDT_FLAG_PRESENT;
    idt[num].base_high = (base >> 16) & 0xFFFF;
}

// ISR handler - called by the assembly ISR stubs
void _isr_handler(registers_t *regs) {
    // Call the registered handler if it exists, otherwise use the default
    if (interrupt_handlers[regs->int_no] != NULL) {
        interrupt_handlers[regs->int_no](regs);
    } else {
        default_interrupt_handler(regs);
    }
}

// IRQ handler - called by the assembly IRQ stubs
void _irq_handler(registers_t *regs) {
    // The IRQ number is stored in regs->int_no - 32
    // Call the ISR handler which will call the appropriate handler
    _isr_handler(regs);
    
    // The EOI is sent by the default handler or the specific IRQ handler
}

// Initialize IDT with IRQ handlers and exceptions
void idt_init(void) {
    // Set up IDT pointer
    idt_ptr_t idt_ptr;
    idt_ptr.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idt_ptr.base = (uint32_t)&idt;
    
    // Clear out the IDT
    memset(&idt, 0, sizeof(idt_entry_t) * IDT_ENTRIES);
    
    // Register exception handlers (ISR 0-31)
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)_isr_handlers[i], 0x08, 0x8E);
    }
    
    // Set up the IRQs
    for (int i = 0; i < 16; i++) {
        idt_set_gate(32 + i, (uint32_t)_irq_handlers[i], 0x08, 0x8E);
    }
    
    // Load the IDT
    _idt_flush((uint32_t)&idt_ptr);
    
    // Initialize all interrupt handlers to NULL (will be set by drivers)
    for (int i = 0; i < IDT_ENTRIES; i++) {
        interrupt_handlers[i] = NULL;
    }
    
    // Don't enable interrupts here - let the kernel_main do that
    // after all drivers are initialized
}
