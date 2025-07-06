; Minimal x86 bootloader (MBR, 512 bytes)
; Loads kernel from disk to 0x1000 and jumps to it

BITS 16
ORG 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load 15 sectors (kernel) to 0x1000
    mov si, 0x1000
    mov bx, si
    mov dh, 15          ; sectors to read
    mov dl, [BOOT_DRIVE]
    mov ah, 0x02        ; INT 13h read sectors
    mov al, dh
    mov ch, 0           ; cylinder
    mov cl, 2           ; sector (start at 2)
    mov dh, 0           ; head
    int 0x13
    jc disk_error

    ; Jump to loaded kernel
    jmp 0x0000:0x1000

disk_error:
    mov si, disk_msg
    call print_string
    jmp $

print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

disk_msg db 'Disk error!', 0

BOOT_DRIVE: db 0

; Boot signature
TIMES 510-($-$$) db 0
DW 0xAA55
