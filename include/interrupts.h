#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <stdbool.h>
#include "kernel/io.h"

// Number of IDT entries
#define IDT_ENTRIES 256

// IDT entry structure
struct idt_entry {
    uint16_t base_low;     // The lower 16 bits of the address to jump to
    uint16_t sel;          // Kernel segment selector
    uint8_t always0;       // This must always be zero
    uint8_t flags;         // More flags
    uint16_t base_high;    // The upper 16 bits of the address to jump to
} __attribute__((packed));

typedef struct idt_entry idt_entry_t;

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

typedef struct idt_ptr idt_ptr_t;

/**
 * @brief CPU registers structure pushed by the CPU during an interrupt
 */
typedef struct registers {
    // Data segment selector (pushed by us)
    uint32_t ds;
    
    // Pushed by pusha
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    
    // Interrupt number and error code (if applicable)
    uint32_t int_no, err_code;
    
    // Pushed by the CPU automatically
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

// Interrupt service routine handler type
typedef void (*isr_t)(registers_t*);

// IDT and ISR function declarations
extern void idt_init(void);
extern void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
extern void register_interrupt_handler(uint8_t n, isr_t handler);
extern void _isr_handler(registers_t *regs);
extern void _irq_handler(registers_t *regs);
void default_interrupt_handler(registers_t *r);

// Assembly functions
extern void _idt_flush(uint32_t ptr);

// ISR handlers
extern void _isr0(void);
extern void _isr1(void);
extern void _isr2(void);
extern void _isr3(void);
extern void _isr4(void);
extern void _isr5(void);
extern void _isr6(void);
extern void _isr7(void);
extern void _isr8(void);
extern void _isr9(void);
extern void _isr10(void);
extern void _isr11(void);
extern void _isr12(void);
extern void _isr13(void);
extern void _isr14(void);
extern void _isr15(void);
extern void _isr16(void);
extern void _isr17(void);
extern void _isr18(void);
extern void _isr19(void);
extern void _isr20(void);
extern void _isr21(void);
extern void _isr22(void);
extern void _isr23(void);
extern void _isr24(void);
extern void _isr25(void);
extern void _isr26(void);
extern void _isr27(void);
extern void _isr28(void);
extern void _isr29(void);
extern void _isr30(void);
extern void _isr31(void);

// IRQ handlers
extern void _irq0(void);
extern void _irq1(void);
extern void _irq2(void);
extern void _irq3(void);
extern void _irq4(void);
extern void _irq5(void);
extern void _irq6(void);
extern void _irq7(void);
extern void _irq8(void);
extern void _irq9(void);
extern void _irq10(void);
extern void _irq11(void);
extern void _irq12(void);
extern void _irq13(void);
extern void _irq14(void);
extern void _irq15(void);

// Assembly stubs
extern void isr_common_stub(void);
extern void irq_common_stub(void);

// ISR stubs (0-31)
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

// IRQ stubs (32-47)
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// IDT flags
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0   0x00
#define IDT_FLAG_RING3   0x60
#define IDT_FLAG_32BIT   0x08
#define IDT_FLAG_16BIT   0x00
#define IDT_FLAG_TRAP    0x0F
#define IDT_FLAG_INTR    0x0E

// Number of IDT entries
#define IDT_ENTRIES 256

// IRQ numbers (mapped to ISR 32-47)
#define IRQ0  32  // Programmable Interrupt Timer
#define IRQ1  33  // Keyboard
#define IRQ2  34  // Cascade (used internally by the two PICs)
#define IRQ3  35  // COM2 (if enabled)
#define IRQ4  36  // COM1 (if enabled)
#define IRQ5  37  // LPT2 (if enabled) or sound card
#define IRQ6  38  // Floppy disk
#define IRQ7  39  // LPT1 / Unreliable "spurious" interrupt (usually)
#define IRQ8  40  // CMOS real-time clock (if enabled)
#define IRQ9  41  // Free for peripherals / legacy SCSI / NIC
#define IRQ10 42  // Free for peripherals / SCSI / NIC
#define IRQ11 43  // Free for peripherals / SCSI / NIC
#define IRQ12 44  // PS2 mouse
#define IRQ13 45  // FPU / Coprocessor / Inter-processor
#define IRQ14 46  // Primary ATA
#define IRQ15 47  // Secondary ATA

// Exception numbers (ISR 0-31)
#define DIVIDE_BY_ZERO         0
#define DEBUG                  1
#define NON_MASKABLE_INTERRUPT 2
#define BREAKPOINT             3
#define OVERFLOW               4
#define BOUND_RANGE_EXCEEDED   5
#define INVALID_OPCODE         6
#define DEVICE_NOT_AVAILABLE   7
#define DOUBLE_FAULT           8
#define COPROCESSOR_SEG_OVERRUN 9
#define INVALID_TSS           10
#define SEGMENT_NOT_PRESENT   11
#define STACK_SEGMENT_FAULT   12
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT            14
#define RESERVED              15
#define FLOATING_POINT_EXCEPTION 16
#define ALIGNMENT_CHECK       17
#define MACHINE_CHECK         18
#define SIMD_FLOATING_POINT   19
#define VIRTUALIZATION        20
#define CONTROL_PROTECTION    21
#define RESERVED_START        22
#define RESERVED_END          31

// Interrupt control functions
static inline void cli(void) { asm volatile("cli"); }
static inline void sti(void) { asm volatile("sti"); }

// Helper macros for interrupt control
#define IRQ_OFF() cli()
#define IRQ_ON()  sti()

// Critical section helper
#define CRITICAL_SECTION(code) \
    do { \
        uint32_t __flags; \
        asm volatile ("pushfl\n\t" "popl %0" : "=r" (__flags) :: "memory"); \
        cli(); \
        { code; } \
        asm volatile ("pushl %0\n\t" "popfl" :: "r" (__flags) : "memory", "cc"); \
    } while(0)

// External declarations for ISR stubs
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

// External declarations for IRQ stubs
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

// IDT and IDT pointer variables
extern idt_entry_t idt[IDT_ENTRIES];
extern idt_ptr_t idtp;

#endif // INTERRUPTS_H
