# Kernel Makefile

# Cross-compiler settings
CC = gcc
AS = nasm
LD = ld
CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib -nostdinc -fno-builtin -fno-common -fno-pic -Wall -Wextra -Werror -Iinclude -g
ASFLAGS = -f win32 -g
LDFLAGS = -m i386pe -T linker.ld -nostdlib --entry=_start --oformat=pei-i386 -Map=kernel.map

# Detect Windows
ifeq ($(OS),Windows_NT)
    RM = del /f /q
    MKDIR = mkdir
    RMDIR = rmdir /s /q
    CP = copy /y
    FIXPATH = $(subst /,\,$1)
    OBJDIR = obj
    KERNEL_OBJDIR = $(OBJDIR)/kernel
    DRIVER_OBJDIR = $(OBJDIR)/drivers
    LIBC_OBJDIR = $(OBJDIR)/libc
else
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    CP = cp -f
    FIXPATH = $1
    OBJDIR = obj
    KERNEL_OBJDIR = $(OBJDIR)/kernel
    DRIVER_OBJDIR = $(OBJDIR)/drivers
    LIBC_OBJDIR = $(OBJDIR)/libc
endif

# Source directories
KERNEL_SRCDIR = kernel
DRIVER_SRCDIR = drivers
LIBC_SRCDIR = libc

# Output files
KERNEL = kernel.bin
KERNEL_IMG = kernel.img

# Find all source files
KERNEL_C_SRCS = $(wildcard $(KERNEL_SRCDIR)/*.c)
KERNEL_ASM_SRCS = $(KERNEL_SRCDIR)/start.asm $(KERNEL_SRCDIR)/interrupts.asm
DRIVER_C_SRCS = $(wildcard $(DRIVER_SRCDIR)/*.c)

# Object files - explicitly list all required object files in the correct order
# Note: The order here is critical for proper linking
# Kernel object files in specific order
KERNEL_OBJS = \
    $(KERNEL_OBJDIR)/start.o \
    $(KERNEL_OBJDIR)/interrupts_asm.o \
    $(KERNEL_OBJDIR)/interrupts.o \
    $(KERNEL_OBJDIR)/pic.o \
    $(KERNEL_OBJDIR)/irq_dispatch.o \
    $(KERNEL_OBJDIR)/kernel.o \
    $(KERNEL_OBJDIR)/kprint.o \
    $(KERNEL_OBJDIR)/main.o \
    $(KERNEL_OBJDIR)/mm.o \
    $(KERNEL_OBJDIR)/panic.o \
    $(LIBC_OBJDIR)/string.o \
    $(DRIVER_OBJDIR)/keyboard.o \
    $(DRIVER_OBJDIR)/serial.o \
    $(DRIVER_OBJDIR)/timer.o \
    $(DRIVER_OBJDIR)/vga.o

# Explicitly list all source files to ensure they're built
KERNEL_SOURCES = \
    kernel/start.asm \
    kernel/interrupts.asm \
    kernel/pic.c \
    kernel/irq_dispatch.c \
    kernel/kernel.c \
    kernel/kprint.c \
    kernel/main.c \
    kernel/mm.c \
    kernel/panic.c \
    kernel/interrupts.c \
    drivers/keyboard.c \
    drivers/serial.c \
    drivers/timer.c \
    drivers/vga.c

# Default target
all: $(KERNEL_IMG)

# Create output directories
$(KERNEL_OBJDIR):
	@$(MKDIR) $(call FIXPATH,$@)

# Rule for start.asm
$(KERNEL_OBJDIR)/start.o: $(KERNEL_SRCDIR)/start.asm | $(KERNEL_OBJDIR)
	@echo "AS $<"
	@$(AS) -f win32 $< -o $@

# Rule for interrupts.asm
$(KERNEL_OBJDIR)/interrupts_asm.o: $(KERNEL_SRCDIR)/interrupts.asm | $(KERNEL_OBJDIR)
	@echo "AS $< (as interrupts_asm.o)"
	@$(AS) -f win32 $< -o $@

# Rule for interrupts.c
$(KERNEL_OBJDIR)/interrupts.o: $(KERNEL_SRCDIR)/interrupts.c | $(KERNEL_OBJDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for libc string.c
$(LIBC_OBJDIR)/string.o: $(LIBC_SRCDIR)/string.c | $(LIBC_OBJDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Ensure libc directory exists
$(LIBC_OBJDIR):
	@$(MKDIR) $(call FIXPATH,$@)

# Ensure interrupts.o is built before other objects that depend on it
$(KERNEL_OBJDIR)/pic.o $(KERNEL_OBJDIR)/irq_dispatch.o $(KERNEL_OBJDIR)/kernel.o: $(KERNEL_OBJDIR)/interrupts.o

# Rule for compiling kernel C files
$(KERNEL_OBJDIR)/%.o: $(KERNEL_SRCDIR)/%.c | $(KERNEL_OBJDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Rule for compiling driver C files
$(DRIVER_OBJDIR)/%.o: $(DRIVER_SRCDIR)/%.c | $(DRIVER_OBJDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# Create driver object directory
$(DRIVER_OBJDIR):
	@$(MKDIR) $(call FIXPATH,$@)

# Compile driver C files
$(DRIVER_OBJDIR)/%.o: $(DRIVER_SRCDIR)/%.c | $(DRIVER_OBJDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -Iinclude -c $< -o $@

# Link kernel
$(KERNEL): $(KERNEL_OBJS)
	@echo "Linking $@..."
	@echo "Object files: $(KERNEL_OBJS)"
	@$(LD) -m i386pe -T linker.ld -nostdlib --oformat=pei-i386 -o $@ $(KERNEL_OBJS) -Map=kernel.map

# Create kernel image
$(KERNEL_IMG): $(KERNEL)
	@echo "Creating $@..."
	@$(CP) $(call FIXPATH,$(KERNEL)) $(call FIXPATH,$@)

# Clean build artifacts
clean:
	@echo "Cleaning..."
	@if exist $(call FIXPATH,$(OBJDIR)) $(RMDIR) $(call FIXPATH,$(OBJDIR))
	@if exist $(call FIXPATH,$(KERNEL)) $(RM) $(call FIXPATH,$(KERNEL))
	@if exist $(call FIXPATH,$(KERNEL_IMG)) $(RM) $(call FIXPATH,$(KERNEL_IMG))

# Run the kernel in QEMU
run: $(KERNEL_IMG)
	@echo "Running in QEMU..."
	qemu-system-i386 -kernel $(KERNEL)

.PHONY: all clean run
