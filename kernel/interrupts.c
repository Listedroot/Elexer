#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../include/kernel/io.h"
#include "../include/interrupts.h"
#include "../drivers/serial.h"

// External declarations for assembly functions
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

extern void irq0_handler(void);
extern void irq1_handler(void);
extern void irq2_handler(void);
extern void irq3_handler(void);
extern void irq4_handler(void);
extern void irq5_handler(void);
extern void irq6_handler(void);
extern void irq7_handler(void);
extern void irq8_handler(void);
extern void irq9_handler(void);
extern void irq10_handler(void);
extern void irq11_handler(void);
extern void irq12_handler(void);
extern void irq13_handler(void);
extern void irq14_handler(void);
extern void irq15_handler(void);

// External declarations for assembly variables
extern uint32_t isr_stub_table[];
extern uint32_t irq_stub_table[];

// Global IDT and IDT pointer
__attribute__((aligned(0x10)))
idt_entry_t idt[IDT_ENTRIES];

static idt_ptr_t idtp;

// Array of exception messages
const char* exception_messages[32] = {
    "Division by zero",
    "Debug",
    "Non-maskable interrupt",
    "Breakpoint",
    "Overflow",
    "Bound range exceeded",
    "Invalid opcode",
    "Device not available",
    "Double fault",
    "Coprocessor segment overrun",
    "Invalid TSS",
    "Segment not present",
    "Stack-segment fault",
    "General protection fault",
    "Page fault",
    "Reserved",
    "x87 floating-point exception",
    "Alignment check",
    "Machine check",
    "SIMD floating-point exception",
    "Virtualization exception",
    "Control protection exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor injection exception",
    "VMM communication exception",
    "Security exception",
    "Reserved"
};

// Forward declarations for assembly stubs
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

// External declarations for IRQ handlers
extern void irq0_handler(void);
extern void irq1_handler(void);
extern void irq2_handler(void);
extern void irq3_handler(void);
extern void irq4_handler(void);
extern void irq5_handler(void);
extern void irq6_handler(void);
extern void irq7_handler(void);
extern void irq8_handler(void);
extern void irq9_handler(void);
extern void irq10_handler(void);
extern void irq11_handler(void);
extern void irq12_handler(void);
extern void irq13_handler(void);
extern void irq14_handler(void);
extern void irq15_handler(void);

// ISR stub table
extern uint32_t isr_stub_table[32];

// IRQ stub table
extern uint32_t irq_stub_table[16];

// Ports for PIC
#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

// PIC command and data ports
#define PIC_EOI 0x20        // End-of-interrupt command code

// IDT entry flags
#define IDT_FLAG_PRESENT (1 << 7)
#define IDT_FLAG_RING0   (0 << 5)
#define IDT_FLAG_32BIT   (0xE << 0)

// IDT entry and pointer types are defined in interrupts.h

extern void idt_flush(uint32_t);

// inb and outb are now provided by io.h

// PIC remap function — remaps IRQs 0-15 to interrupts 32-47
void pic_remap(void) {
    // Save masks
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    // Start initialization sequence (cascade mode)
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);
    
    // ICW2: Set vector offsets
    outb(PIC1_DATA, 0x20);  // IRQ0-7 mapped to 0x20-0x27
    outb(PIC2_DATA, 0x28);  // IRQ8-15 mapped to 0x28-0x2F
    
    // ICW3: Configure cascading
    outb(PIC1_DATA, 0x04);  // Tell Master PIC about slave at IRQ2 (0000 0100)
    outb(PIC2_DATA, 0x02);  // Tell Slave PIC its cascade identity (0000 0010)
    
    // ICW4: Set x86 mode
    outb(PIC1_DATA, 0x01);  // 8086/88 mode
    outb(PIC2_DATA, 0x01);
    
    // Restore masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

// Send EOI to PICs
void pic_send_eoi(uint8_t irq) {
    // Send EOI to slave PIC if this was an IRQ from it
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    // Always send EOI to master PIC
    outb(PIC1_COMMAND, PIC_EOI);
}

// Set an entry in the IDT
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags | IDT_FLAG_PRESENT;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
}

// ISR handler - called by the assembly ISR stubs
void isr_handler(uint32_t int_no, uint32_t err_code) {
    // Handle the exception
    if (int_no < 32) {
        // Log the exception
        serial_write_string(SERIAL_COM1_BASE, "\nKERNEL PANIC: ");
        serial_write_string(SERIAL_COM1_BASE, exception_messages[int_no]);
        serial_write_string(SERIAL_COM1_BASE, " (");
        serial_write_hex(SERIAL_COM1_BASE, int_no);
        serial_write_string(SERIAL_COM1_BASE, ") at 0x");
        
        // Get the instruction pointer from the stack
        uint32_t eip;
        asm volatile ("mov 4(%%esp), %0" : "=r" (eip));
        serial_write_hex(SERIAL_COM1_BASE, eip);
        
        // If there's an error code, print it
        if (err_code != 0xFFFFFFFF) {
            serial_write_string(SERIAL_COM1_BASE, " Error code: 0x");
            serial_write_hex(SERIAL_COM1_BASE, err_code);
        }
        
        serial_write_string(SERIAL_COM1_BASE, "\n");
        
        // Halt the system
        asm volatile ("cli; hlt");
    }
}

// Initialize IDT with IRQ handlers and exceptions
void idt_init(void) {
    // Remap the PIC
    pic_remap();

    // Clear out the entire IDT, initializing it to zeros
    memset(&idt, 0, sizeof(idt_entry_t) * IDT_ENTRIES);

    // Initialize the IDT pointer
    idtp.limit = sizeof(idt_entry_t) * IDT_ENTRIES - 1;
    idtp.base = (uint32_t)&idt;

    // Set up the exception handlers (ISRs 0-31)
    for (int i = 0; i < 32; i++) {
        idt_set_gate(i, (uint32_t)isr_stub_table[i], 0x08,
                    IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_32BIT);
    }

    // Set up the IRQ handlers (ISRs 32-47)
    for (int i = 0; i < 16; i++) {
        idt_set_gate(32 + i, (uint32_t)irq_stub_table[i], 0x08,
                    IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_32BIT);
    }

    // Load the IDT
    asm volatile("lidt %0" : : "m" (idtp));

    // Enable interrupts
    asm volatile("sti");
}

extern void irq_handler_common(uint8_t irq);

// IRQ handler - called by the assembly IRQ stubs
void irq_handler(uint8_t irq) {
    // Call the common IRQ handler
    irq_handler_common(irq);
    
    // Send EOI to the PICs
    pic_send_eoi(irq);
}

// Common IRQ handler implementation
void irq_handler_common(uint8_t irq) {
    // Handle the IRQ
    if (irq >= 8) {
        // Send EOI to slave PIC
        outb(0xA0, 0x20);
    }
    // Send EOI to master PIC
    outb(0x20, 0x20);
}
