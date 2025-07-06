; Interrupt Service Routines (ISRs) and Interrupt Request (IRQ) handlers

; Macro to push all registers and the error code
%macro ISR_NOERRCODE 1
    global _isr%1
    _isr%1:
        cli
        push byte 0          ; Push a dummy error code
        push byte %1         ; Push the interrupt number
        jmp _isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
    global _isr%1
    _isr%1:
        cli
        push byte %1         ; The interrupt number is already on the stack
        jmp _isr_common_stub
%endmacro

; Macro for IRQ handlers
%macro IRQ 2
    global _irq%1
    _irq%1:
        cli
        push byte 0
        push byte %2
        jmp _irq_common_stub
%endmacro

; Define ISRs (exceptions)
ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; Define IRQs
IRQ 0,  32
IRQ 1,  33
IRQ 2,  34
IRQ 3,  35
IRQ 4,  36
IRQ 5,  37
IRQ 6,  38
IRQ 7,  39
IRQ 8,  40
IRQ 9,  41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

; External C functions
extern _isr_handler
extern _irq_handler

global _isr_handlers
global _irq_handlers
global _idt_flush
global _isr_common_stub
global _irq_common_stub



; Export all ISR and IRQ handlers
%assign i 0
%rep 32
    global _isr%+i
    %assign i i+1
%endrep

%assign i 0
%rep 16
    global _irq%+i
    %assign i i+1
%endrep

section .data
; Array of ISR handler addresses
_isr_handlers:
    dd _isr0, _isr1, _isr2, _isr3, _isr4, _isr5, _isr6, _isr7
    dd _isr8, _isr9, _isr10, _isr11, _isr12, _isr13, _isr14, _isr15
    dd _isr16, _isr17, _isr18, _isr19, _isr20, _isr21, _isr22, _isr23
    dd _isr24, _isr25, _isr26, _isr27, _isr28, _isr29, _isr30, _isr31

; Array of IRQ handler addresses
_irq_handlers:
    dd _irq0, _irq1, _irq2, _irq3, _irq4, _irq5, _irq6, _irq7
    dd _irq8, _irq9, _irq10, _irq11, _irq12, _irq13, _irq14, _irq15
global _isr_handlers
global _irq_handlers

section .text

; Common ISR stub
global _isr_common_stub
_isr_common_stub:
    ; Save all registers
    pusha
    
    ; Save the data segment
    mov ax, ds
    push eax
    
    ; Load the kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call the C handler
    push eax
    push esp
    call _isr_handler
    add esp, 4
    pop eax
    
    ; Restore the data segment
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    ; Restore all registers
    popa
    
    ; Clean up the error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret

; Common IRQ stub
_irq_common_stub:
    ; Save all registers
    pusha
    
    ; Save the data segment
    mov ax, ds
    push eax
    
    ; Load the kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call the C handler
    push eax
    push esp
    call _irq_handler
    add esp, 4
    pop eax
    
    ; Restore the data segment
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    ; Restore all registers
    popa
    
    ; Clean up the error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret

; Function to load the IDT
global _idt_flush
_idt_flush:
    mov eax, [esp+4]    ; Get the pointer to the IDT
    lidt [eax]          ; Load the IDT
    ret
    mov eax, [esp+4]    ; Get the pointer to the IDT
    lidt [eax]          ; Load the IDT
    ret
