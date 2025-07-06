# ElexerKER

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![Platform: x86](https://img.shields.io/badge/platform-x86-ff69b4)]()
[![Language: C/ASM](https://img.shields.io/badge/language-C%2FASM-0052cf)]()

A minimal x86 kernel written in C and assembly, designed for educational purposes and system programming exploration.

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Build Instructions](#build-instructions)
- [Project Structure](#project-structure)
- [Running the Kernel](#running-the-kernel)
- [Development](#development)
- [License](#license)

## Features

- **32-bit Protected Mode** implementation
- **Memory Management** with basic paging support
- **Hardware Abstraction** through modular drivers
- **Interrupt Handling** with IDT and ISR support
- **VGA Text Mode** display driver
- **PS/2 Keyboard** input driver
- **Basic Shell** for user interaction
- **Minimal C Library** for kernel development

## Prerequisites

- **Windows OS** (Windows 10/11 recommended)
- **MinGW-w64** (with GCC and LD)
- **NASM** (Netwide Assembler) 2.15+
- **QEMU** (for emulation)
- **Make** (for build automation)

## Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/elexerker.git
   cd elexerker
   ```

2. Build the project:
   ```bash
   make
   ```
   
   This will:
   - Clean previous build artifacts
   - Compile the bootloader and kernel
   - Link all components
   - Generate a bootable disk image
   - Optionally launch QEMU if available

## Project Structure

```
elexerKER/
│
├── boot/           # Bootloader and multiboot header
├── drivers/        # Hardware drivers
│   ├── keyboard/   # PS/2 keyboard driver
│   ├── serial/     # Serial port driver
│   ├── timer/      # Programmable Interval Timer (PIT)
│   └── vga/        # VGA text mode driver
│
├── include/        # Header files
│   ├── kernel/     # Kernel headers
│   └── drivers/    # Driver headers
│
├── kernel/         # Kernel source code
│   ├── cpu/        # CPU-related code
│   ├── interrupts/ # Interrupt handling
│   ├── memory/     # Memory management
│   └── shell/      # Basic shell implementation
│
├── libc/           # Minimal C library
│   ├── stdio/      # Standard I/O functions
│   ├── stdlib/     # Standard library functions
│   └── string/     # String manipulation
│
└── network/        # Network stack (WIP)
```

## Running the Kernel

### Using QEMU (Recommended)
```bash
qemu-system-i386 -fda kernel.img -serial stdio
```

### Using Physical Hardware
1. Write the `kernel.img` to a USB drive:
   ```bash
   dd if=kernel.img of=/dev/sdX bs=4M status=progress
   ```
2. Boot from the USB drive

## Development

### Debugging
To debug the kernel with GDB and QEMU:
```bash
qemu-system-i386 -s -S -fda kernel.img &
gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
```

### Code Style
- Follow the Linux kernel coding style for C code
- Use descriptive variable and function names
- Include function documentation headers
- Keep lines under 80 characters

## License

Distributed under the MIT License. See `LICENSE` for more information.

---

**Note**: This project is for educational purposes only. Use at your own risk.
