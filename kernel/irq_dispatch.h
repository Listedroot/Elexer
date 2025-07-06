#ifndef IRQ_DISPATCH_H
#define IRQ_DISPATCH_H

#include "../include/interrupts.h"

// Initialize the IRQ subsystem
void irq_init(void);

// Default handler for unhandled IRQs
void default_irq_handler(registers_t *regs);

#endif // IRQ_DISPATCH_H
