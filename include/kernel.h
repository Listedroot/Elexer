#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Kernel version
#define KERNEL_NAME    "ElexerKernel"
#define KERNEL_VERSION "0.1.0"

// Kernel panic handler
void panic(const char* message);

// Kernel entry point (defined in kernel.c)
void kernel_main(void);

// Basic I/O functions
void kprint(const char* str);
void kputc(char c);
void kclear(void);

// Memory management
void* kmalloc(size_t size);
void kfree(void* ptr);

// Utility functions
void halt(void);
void cli(void);
void sti(void);

#endif // KERNEL_H
