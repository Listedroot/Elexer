#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// IDT size
#define IDT_ENTRIES 256

// IDT entry flags
#define IDT_FLAG_PRESENT   (1 << 7)  // 10000000b - Present bit
#define IDT_FLAG_RING0     (0 << 5)  // 00000000b - Ring 0 (kernel mode)
#define IDT_FLAG_RING3     (3 << 5)  // 01100000b - Ring 3 (user mode)
#define IDT_FLAG_32BIT     (0xE << 0) // 00001110b - 32-bit interrupt gate
#define IDT_FLAG_16BIT     (0x6 << 0) // 00000110b - 16-bit interrupt gate
#define IDT_FLAG_TRAP      (0xF << 0) // 00001111b - 32-bit trap gate

// IDT entry structure
typedef struct {
    uint16_t base_lo;    // Lower 16 bits of the handler address
    uint16_t sel;        // Kernel segment selector
    uint8_t always0;     // Always 0
    uint8_t flags;       // Flags (present, DPL, type)
    uint16_t base_hi;    // Upper 16 bits of the handler address
} __attribute__((packed)) idt_entry_t;

// IDT pointer structure
typedef struct {
    uint16_t limit;      // Size of IDT in bytes - 1
    uint32_t base;       // Linear address of the IDT
} __attribute__((packed)) idt_ptr_t;

// Interrupt service routine handler type
typedef void (*isr_t)(void);

// Exception handler type
typedef void (*exception_handler_t)(uint32_t int_no, uint32_t err_code);

// Initialize the IDT and install default handlers
void idt_init(void);

// Global IDT and IDT pointer
extern idt_entry_t idt[IDT_ENTRIES];
extern idt_ptr_t idtp;

// Set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Initialize the PIC and remap IRQs
void pic_remap(void);

// Send EOI to PIC
void pic_send_eoi(uint8_t irq);

// Register an interrupt handler
void register_interrupt_handler(uint8_t num, isr_t handler);

// Register an exception handler
void register_exception_handler(uint8_t num, exception_handler_t handler);

// ISR handler
void isr_handler(uint32_t int_no, uint32_t err_code);

// IRQ handler common function
void irq_handler(uint8_t irq);

// IRQ handler common implementation
void irq_handler_common(uint8_t irq);

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

// Enable interrupts
static inline void enable_interrupts(void) {
    asm volatile ("sti");
}

// Disable interrupts
static inline void disable_interrupts(void) {
    asm volatile ("cli");
}


// Halt the CPU until the next interrupt
static inline void halt_cpu(void) {
    asm volatile ("hlt");
}

// IRQ numbers (PIC1)
#define IRQ0  32  // Timer
#define IRQ1  33  // Keyboard
#define IRQ2  34  // Cascade (used internally by the two PICs)
#define IRQ3  35  // COM2 (if enabled)
#define IRQ4  36  // COM1 (if enabled)
#define IRQ5  37  // LPT2 (if enabled)
#define IRQ6  38  // Floppy disk
#define IRQ7  39  // LPT1 / Unreliable "spurious" interrupt (usually)

// IRQ numbers (PIC2)
#define IRQ8  40  // CMOS real-time clock (if enabled)
#define IRQ9  41  // Free for peripherals / legacy SCSI / NIC
#define IRQ10 42  // Free for peripherals / SCSI / NIC
#define IRQ11 43  // Free for peripherals / SCSI / NIC
#define IRQ12 44  // PS/2 mouse
#define IRQ13 45  // FPU / Coprocessor / Inter-processor
#define IRQ14 46  // Primary ATA hard disk
#define IRQ15 47  // Secondary ATA hard disk

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

// Global IDT and IDT pointer
extern idt_entry_t idt[IDT_ENTRIES];
extern idt_ptr_t idtp;

// Exception messages
extern const char* exception_messages[32];

// Exception numbers
#define DIVIDE_BY_ZERO 0
#define DEBUG_EXCEPTION 1
#define NON_MASKABLE_INTERRUPT 2
#define BREAKPOINT 3
#define OVERFLOW 4
#define BOUND_RANGE_EXCEEDED 5
#define INVALID_OPCODE 6
#define DEVICE_NOT_AVAILABLE 7
#define DOUBLE_FAULT 8
#define COPROCESSOR_SEGMENT_OVERRUN 9
#define INVALID_TSS 10
#define SEGMENT_NOT_PRESENT 11
#define STACK_SEGMENT_FAULT 12
#define GENERAL_PROTECTION_FAULT 13
#define PAGE_FAULT 14
#define X87_FPU_EXCEPTION 16
#define ALIGNMENT_CHECK 17
#define MACHINE_CHECK 18
#define SIMD_FLOATING_POINT_EXCEPTION 19
#define VIRTUALIZATION_EXCEPTION 20
#define SECURITY_EXCEPTION 30

#ifdef __cplusplus
}
#endif

#endif // INTERRUPTS_H
