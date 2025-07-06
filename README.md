# ElexerKER - A Minimal x86 Kernel

ElexerKER is a minimal x86 kernel written in C and assembly, designed for educational purposes.

## Prerequisites

- Windows OS
- MinGW-w64 (with GCC and LD)
- NASM
- QEMU (for running the kernel)

## Building the Kernel

1. Open a command prompt in the project directory
2. Run the build script:
   ```
   build.bat
   ```

This will:
1. Clean previous build artifacts
2. Compile the bootloader
3. Compile the kernel and drivers
4. Link everything together
5. Create a bootable disk image
6. Optionally start QEMU if installed

## Project Structure

```
elexerKER/
├── boot/           # Bootloader code
├── drivers/        # Hardware drivers
├── include/        # Header files
│   ├── kernel/     # Kernel headers
│   └── drivers/    # Driver headers
├── kernel/         # Kernel source code
├── libc/           # Minimal C library
└── network/        # Network stack
```

## Running the Kernel

If QEMU is installed and in your PATH, the build script will automatically start it. Otherwise, you can run:

```
qemu-system-i386 -fda kernel.img
```

## Key Features

- 32-bit protected mode
- Basic memory management
- VGA text mode output
- Keyboard input
- Interrupt handling
- Basic shell
- Minimal C library

## License

This project is licensed under the MIT License - see the LICENSE file for details.
