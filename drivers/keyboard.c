#include "keyboard.h"
#include "../libc/string.h"
#include "vga.h"   // optional for debug prints
#include <stdint.h>
#include <stdbool.h>

#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64
#define KBD_CMD_PORT    0x64

#define KBD_BUFFER_SIZE 128

static volatile char key_buffer[KBD_BUFFER_SIZE];
static volatile int head = 0;
static volatile int tail = 0;

// Scancode to ASCII (partial US layout, no shift support for simplicity)
static const char scancode_to_ascii[] = {
    0, 27, '1','2','3','4','5','6','7','8',
    '9','0','-','=', '\b','\t','q','w','e','r',
    't','y','u','i','o','p','[',']', '\n', 0,
    'a','s','d','f','g','h','j','k','l',';',
    '\'','`', 0, '\\','z','x','c','v','b','n',
    'm',',','.','/', 0, '*', 0, ' ', 0, 0,
    // rest zeros
};

// inb and outb are defined in kernel/io.h

// Put char into circular buffer
static void buffer_put(char c) {
    int next = (head + 1) % KBD_BUFFER_SIZE;
    if (next != tail) {
        key_buffer[head] = c;
        head = next;
    }
}

// Get char from circular buffer, return 0 if empty
static char buffer_get(void) {
    if (head == tail) return 0; // empty
    char c = key_buffer[tail];
    tail = (tail + 1) % KBD_BUFFER_SIZE;
    return c;
}

// Interrupt handler, called from IRQ1
void keyboard_handler(registers_t *regs)
{
    (void)regs; // Mark as unused to prevent compiler warning
    
    uint8_t status = inb(KBD_STATUS_PORT);
    if (status & 0x01) {
        uint8_t scancode = inb(KBD_DATA_PORT);
        
        // Only handle key press events (ignore key releases for now)
        if (scancode < 0x80) {
            char c = scancode_to_ascii[scancode];
            if (c) {
                buffer_put(c);
            }
        }
    }
}

// Initialize keyboard (enable IRQ1)
void keyboard_init(void) {
    // PIC remapping done elsewhere by whoever owns IRQs
    // Just enable IRQ1 on PIC1 mask register
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1); // clear mask bit 1 (IRQ1 keyboard)
    outb(0x21, mask);
}

// Blocking read for one char
char keyboard_get_char(void) {
    char c = 0;
    while (!(c = buffer_get())) {
        // optionally halt lol CPU to save power until interrupt fires
        __asm__ volatile("hlt");
    }
    return c;
}

// Non-blocking check if char is available
int keyboard_has_char(void) {
    return head != tail;
}
// :sob:..:skull: