; Kernel entry point
[BITS 32]
[GLOBAL _start]

section .text
_start:
    ; Set up stack
    mov esp, stack_top
    
    ; Clear direction flag (for string operations)
    cld
    
    ; Call kernel main
    extern _kernel_main
    call _kernel_main
    
    ; Halt if kernel_main returns
    cli
    hlt

; Reserve stack space
section .bss
stack_bottom:
    resb 16384  ; 16KB stack
stack_top:
