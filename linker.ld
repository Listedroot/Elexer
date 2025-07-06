/* Linker script for the kernel */

ENTRY(_start)

SECTIONS {
    /* The kernel will be loaded at 1MB in physical memory */
    . = 0x100000;

    /* Kernel code section */
    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.text)
        *(.text.*)
    }

    /* Read-only data */
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
        *(.rodata.*)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
        *(.data.*)
    }

    /* Constructors and destructors */
    .ctors : ALIGN(4) {
        __CTOR_LIST__ = .;
        *(.ctors)
        *(.init_array)
        LONG(0);
        __CTOR_END__ = .;
    }

    .dtors : ALIGN(4) {
        __DTOR_LIST__ = .;
        *(.dtors)
        *(.fini_array)
        LONG(0);
        __DTOR_END__ = .;
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
        *(.bss.*)
        *(.bootstrap_stack)
    }

    /* Define symbols for the end of the kernel */
    . = ALIGN(4K);
    _kernel_end = .;

    /* Remove unnecessary sections */
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
        *(.note.*)
    }
}
