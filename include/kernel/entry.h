#ifndef KERNEL_ENTRY_H
#define KERNEL_ENTRY_H

// Kernel entry point - called by bootloader
void _start(void);

// Kernel main function - called after initialization
void kernel_main(void);

// Kernel panic handler
void panic(const char* message);

// CPU control functions
void halt(void);
void cli(void);
void sti(void);

#endif // KERNEL_ENTRY_H
