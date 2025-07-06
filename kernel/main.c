#include "kernel.h"
#include "config.h"
#include "../drivers/vga.h"
#include "../drivers/serial.h"
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "interrupts.h"

// Kernel entry point - called by bootloader
__attribute__((section(".multiboot")))
__attribute__((noreturn))
void _start(void) {
    // Initialize VGA first for output
    vga_clear();
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_puts("Booting " KERNEL_NAME " v" KERNEL_VERSION "...\n");

    // Initialize serial first (early debugging)
    serial_init(SERIAL_COM1_BASE, 115200);
    serial_write_string(SERIAL_COM1_BASE, "Serial port initialized\n");

    // Initialize hardware devices
    timer_init(PIT_FREQUENCY);
    keyboard_init();
    vga_puts("Hardware initialized\n");

    // Initialize interrupt descriptor table
    idt_init();
    vga_puts("IDT initialized\n");

    // Enable interrupts
    sti();
    vga_puts("Interrupts enabled\n");

    // Call the main kernel function from kernel.c
    _kernel_main();

    // If kernel_main returns, halt the system
    for (;;) {
        __asm__ volatile ("hlt");
    }
}

// Halt the CPU
void halt(void) {
    __asm__ volatile ("hlt");
}

// cli() and sti() are defined in interrupts.h
