# Kernel Makefile

# Cross-compiler settings
CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy
CFLAGS = -m32 -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-common -fno-pic -Wall -Wextra -Werror -Iinclude
ASFLAGS = -f win32
LDFLAGS = -m i386pe -T linker.ld -nostdlib

# Detect Windows
ifeq ($(OS),Windows_NT)
    RM = del /f /q
    MKDIR = mkdir
    RMDIR = rmdir /s /q
    CP = copy /y
    FIXPATH = $(subst /,\,$1)
    DD = fsutil file createnew $(call FIXPATH,$1) $(shell if [ "$2" -gt "0" ]; then echo $2; else echo 1; fi) >nul 2>&1
    APPEND = type $(call FIXPATH,$1) >> $(call FIXPATH,$2)
    SEEK = 
else
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    CP = cp -f
    FIXPATH = $1
    DD = dd if=/dev/zero of=$1 bs=1 count=$2 >/dev/null 2>&1
    APPEND = cat $1 >> $2
    SEEK = seek=1
endif

# Source directories
KERNEL_SRCDIR = kernel
DRIVER_SRCDIR = drivers
LIBC_SRCDIR = libc
BOOT_SRCDIR = boot

# Output files
KERNEL = kernel.bin
KERNEL_IMG = kernel.img
BOOTLOADER = $(BOOT_SRCDIR)/boot.bin

# Source files
KERNEL_SRCS = $(wildcard $(KERNEL_SRCDIR)/*.c) \
              $(wildcard $(KERNEL_SRCDIR)/*.s) \
              $(wildcard $(KERNEL_SRCDIR)/*.asm)

DRIVER_SRCS = $(wildcard $(DRIVER_SRCDIR)/*.c)
LIBC_SRCS = $(wildcard $(LIBC_SRCDIR)/*.c)

# Object files
KERNEL_OBJS = $(patsubst %.c,%.o,$(patsubst %.s,%.o,$(patsubst %.asm,%.o,$(KERNEL_SRCS))))
DRIVER_OBJS = $(patsubst %.c,%.o,$(DRIVER_SRCS))
LIBC_OBJS = $(patsubst %.c,%.o,$(LIBC_SRCS))

# Default target
all: $(KERNEL_IMG)

# Build the kernel image
$(KERNEL_IMG): $(BOOTLOADER) $(KERNEL)
	$(call DD,$@,1474560)  # 1.44MB floppy image
	$(CP) $(call FIXPATH,$(BOOTLOADER)) $(call FIXPATH,$@)
	$(CP) $(call FIXPATH,$(KERNEL)) $(call FIXPATH,temp_kernel.bin)
	$(APPEND,temp_kernel.bin,$@)
	$(RM) temp_kernel.bin

# Build the kernel
$(KERNEL): $(KERNEL_OBJS) $(DRIVER_OBJS) $(LIBC_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# Build the bootloader
$(BOOTLOADER): $(BOOT_SRCDIR)/boot.asm
	$(AS) -f bin $< -o $@

# Pattern rule for compiling C files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pattern rule for assembling ASM files
%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

# Pattern rule for assembling NASM files
%.o: %.asm
	$(AS) -f win32 $< -o $@

# Clean build artifacts
clean:
	$(RM) $(call FIXPATH,$(KERNEL_OBJS) $(DRIVER_OBJS) $(LIBC_OBJS) $(KERNEL) $(KERNEL_IMG) $(BOOTLOADER))

# Run the kernel in QEMU
run: $(KERNEL_IMG)
	qemu-system-i386 -fda $<

.PHONY: all clean run
