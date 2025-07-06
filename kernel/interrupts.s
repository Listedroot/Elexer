; interrupts.s

; Export all ISR stubs (0-31)
%macro EXPORT_ISR 1
global isr%1
%endmacro

%assign i 0
%rep 32
    EXPORT_ISR i
    %assign i i+1
%endrep

; Export all IRQ handlers (0-15)
global irq0_handler
global irq1_handler
global irq2_handler
global irq3_handler
global irq4_handler
global irq5_handler
global irq6_handler
global irq7_handler
global irq8_handler
global irq9_handler
global irq10_handler
global irq11_handler
global irq12_handler
global irq13_handler
global irq14_handler
global irq15_handler

global idt_flush
global isr_stub_table
global irq_stub_table

; Define the ISR stub table in the data section
section .data
align 4
isr_stub_table:
%assign i 0
%rep 32
    dd isr%+i
    %assign i i+1
%endrep

; Define the IRQ stub table in the data section
align 4
irq_stub_table:
    dd irq0_handler
    dd irq1_handler
    dd irq2_handler
    dd irq3_handler
    dd irq4_handler
    dd irq5_handler
    dd irq6_handler
    dd irq7_handler
    dd irq8_handler
    dd irq9_handler
    dd irq10_handler
    dd irq11_handler
    dd irq12_handler
    dd irq13_handler
    dd irq14_handler
    dd irq15_handler

section .text

; External C handler prototypes
extern irq_handler
extern isr_handler

; Function to load the IDT
global idt_load
extern idtp

idt_load:
    lidt [idtp]
    ret

; Common ISR stub code
%macro ISR_NOERRCODE 1  ; No error code pushed by CPU
isr%1:
    cli
    push byte 0         ; Push dummy error code
    push byte %1        ; Push interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1   ; CPU pushes error code
isr%1:
    cli
    push byte %1        ; Push interrupt number
    jmp isr_common_stub
%endmacro

; Common IRQ handler
%macro IRQ 2
irq%1_handler:
    cli
    push byte 0         ; Dummy error code
    push byte %2        ; Interrupt number
    jmp irq_common_stub
%endmacro

; Common ISR handler
isr_common_stub:
    ; Save all registers
    pusha
    
    ; Save the data segment descriptor
    mov ax, ds
    push eax
    
    ; Load the kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call the C handler
    push esp
    call isr_handler
    add esp, 4          ; Clean up stack
    
    ; Restore the data segment descriptor
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    ; Restore registers and return
    popa
    add esp, 8          ; Clean up error code and interrupt number
    iret

; Common IRQ handler
irq_common_stub:
    ; Save all registers
    pusha
    
    ; Save the data segment descriptor
    mov ax, ds
    push eax
    
    ; Load the kernel data segment
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call the C handler
    push esp
    call irq_handler
    add esp, 4          ; Clean up stack
    
    ; Send EOI to PIC
    mov al, 0x20        ; EOI command
    out 0x20, al        ; Send to master PIC
    
    ; Restore the data segment descriptor
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    ; Restore registers and return
    popa
    add esp, 8          ; Clean up error code and interrupt number
    iret

; Define all ISR stubs (0-31)
ISR_NOERRCODE 0    ; Divide by zero
ISR_NOERRCODE 1    ; Debug
ISR_NOERRCODE 2    ; Non-maskable interrupt
ISR_NOERRCODE 3    ; Breakpoint
ISR_NOERRCODE 4    ; Overflow
ISR_NOERRCODE 5    ; Bound range exceeded
ISR_NOERRCODE 6    ; Invalid opcode
ISR_NOERRCODE 7    ; Device not available
ISR_ERRCODE   8    ; Double fault
ISR_NOERRCODE 9    ; Coprocessor segment overrun
ISR_ERRCODE   10   ; Invalid TSS
ISR_ERRCODE   11   ; Segment not present
ISR_ERRCODE   12   ; Stack-segment fault
ISR_ERRCODE   13   ; General protection fault
ISR_ERRCODE   14   ; Page fault
ISR_NOERRCODE 15   ; Reserved
ISR_NOERRCODE 16   ; x87 floating-point exception
ISR_ERRCODE   17   ; Alignment check
ISR_NOERRCODE 18   ; Machine check
ISR_NOERRCODE 19   ; SIMD floating-point exception
ISR_NOERRCODE 20   ; Virtualization exception
ISR_NOERRCODE 21   ; Control protection exception
ISR_NOERRCODE 22   ; Reserved
ISR_NOERRCODE 23   ; Reserved
ISR_NOERRCODE 24   ; Reserved
ISR_NOERRCODE 25   ; Reserved
ISR_NOERRCODE 26   ; Reserved
ISR_NOERRCODE 27   ; Reserved
ISR_NOERRCODE 28   ; Hypervisor injection exception
ISR_ERRCODE   29   ; VMM communication exception
ISR_ERRCODE   30   ; Security exception
ISR_NOERRCODE 31   ; Reserved

; Common IRQ handler prologue/epilogue macro to avoid repetition
%macro IRQ_HANDLER 1
%assign irqnum %1
irq%1_handler:
    cli
    push byte 0         ; Push dummy error code
    push dword irqnum   ; Push interrupt number
    pusha               ; Save all general-purpose registers
    
    ; Save the data segment descriptor
    mov ax, ds
    push eax
    
    ; Load the kernel data segment descriptor
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Call the C handler
    call irq_handler
    
    ; Restore the data segment descriptor
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    ; Restore all general-purpose registers
    popa
    
    ; Clean up the pushed error code and interrupt number
    add esp, 8
    
    ; Return from interrupt
    iret
%endmacro

IRQ_HANDLER 0
IRQ_HANDLER 1
IRQ_HANDLER 2
IRQ_HANDLER 3
IRQ_HANDLER 4
IRQ_HANDLER 5
IRQ_HANDLER 6
IRQ_HANDLER 7
IRQ_HANDLER 8
IRQ_HANDLER 9
IRQ_HANDLER 10
IRQ_HANDLER 11
IRQ_HANDLER 12
IRQ_HANDLER 13
IRQ_HANDLER 14
IRQ_HANDLER 15

; idt_flush - loads the IDT with lidt
; void idt_flush(uint32_t);
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret
