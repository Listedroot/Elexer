#include <stdint.h>
#include "../include/kernel/io.h"

// PIC ports
#define PIC1_CMD    0x20
#define PIC1_DATA   0x21
#define PIC2_CMD    0xA0
#define PIC2_DATA   0xA1

// PIC commands
#define PIC_EOI     0x20        // End-of-interrupt command
#define ICW1_ICW4   0x01        // ICW4 (not) needed
#define ICW1_SINGLE 0x02        // Single (cascade) mode
#define ICW1_INTERVAL4 0x04     // Call address interval 4 (8)
#define ICW1_LEVEL  0x08        // Level triggered (edge) mode
#define ICW1_INIT   0x10        // Initialization - required!

#define ICW4_8086   0x01        // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO   0x02        // Auto (normal) EOI
#define ICW4_BUF_SLAVE 0x08     // Buffered mode/slave
#define ICW4_BUF_MASTER 0x0C    // Buffered mode/master
#define ICW4_SFNM   0x10        // Special fully nested (not)

// Remap the PICs to avoid conflicts with CPU exceptions
void pic_remap(void) {
    // Save masks
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    // Start initialization sequence (cascade mode)
    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    
    // ICW2: Vector offsets
    outb(PIC1_DATA, 0x20);  // Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, 0x28);  // Slave PIC vector offset
    io_wait();
    
    // ICW3: Tell Master PIC about slave at IRQ2 (0000 0100)
    outb(PIC1_DATA, 4);
    io_wait();
    // ICW3: Tell Slave PIC its cascade identity (0000 0010)
    outb(PIC2_DATA, 2);
    io_wait();
    
    // ICW4: 8086/88 mode
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    
    // Restore saved masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

// Send End-Of-Interrupt signal to PICs
void pic_send_eoi(uint8_t irq) {
    if (irq >= 8) {
        outb(PIC2_CMD, PIC_EOI);
    }
    outb(PIC1_CMD, PIC_EOI);
}

// Disable all IRQs
void pic_disable(void) {
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

// Set IRQ mask (0 = enable, 1 = disable)
void pic_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    
    value = inb(port) | (1 << irq_line);
    outb(port, value);
}

// Clear IRQ mask (0 = enable, 1 = disable)
void pic_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if (irq_line < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_line -= 8;
    }
    
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);
}
