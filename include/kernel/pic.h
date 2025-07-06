#ifndef PIC_H
#define PIC_H

#include <stdint.h>

// PIC ports
#define PIC1_CMD    0x20
#define PIC1_DATA   0x21
#define PIC2_CMD    0xA0
#define PIC2_DATA   0xA1

// PIC commands
#define PIC_EOI     0x20        // End-of-interrupt command

// Function declarations
void pic_remap(void);
void pic_send_eoi(uint8_t irq);
void pic_disable(void);
void pic_set_mask(uint8_t irq_line);
void pic_clear_mask(uint8_t irq_line);

#endif // PIC_H
