#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void pic_remap(void);
void pic_send_eoi(uint8_t irq);

void idt_init(void);
void irq_handler(uint8_t irq);

#endif
