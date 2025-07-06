#include "vga.h"
#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t*)0xB8000)

#define VGA_CMD_PORT  0x3D4
#define VGA_DATA_PORT 0x3D5

static uint16_t* const video_mem = VGA_MEMORY;

static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;
static uint8_t vga_color = (VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4));

// Port IO functions
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Create a VGA entry combining char and color
static inline uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

// Update hardware cursor to current pos
static void update_cursor() {
    uint16_t pos = cursor_row * VGA_WIDTH + cursor_col;
    outb(VGA_CMD_PORT, 0x0F);
    outb(VGA_DATA_PORT, (uint8_t)(pos & 0xFF));
    outb(VGA_CMD_PORT, 0x0E);
    outb(VGA_DATA_PORT, (uint8_t)((pos >> 8) & 0xFF));
}

// Scroll screen up by one line
void vga_scroll() {
    // Move lines up ... Cool
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            video_mem[(y - 1) * VGA_WIDTH + x] = video_mem[y * VGA_WIDTH + x];
        }
    }
    // Clear last line
    uint16_t blank = vga_entry(' ', vga_color);
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        video_mem[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = blank;
    }

    if (cursor_row > 0)
        cursor_row--;
}

// Clear entire screen
void vga_clear() {
    uint16_t blank = vga_entry(' ', vga_color);
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        video_mem[i] = blank;
    }
    cursor_row = 0;
    cursor_col = 0;
    update_cursor();
}

// Set foreground and background color
void vga_set_color(uint8_t fg, uint8_t bg) {
    vga_color = (fg & 0x0F) | ((bg & 0x0F) << 4);
}

// Get current foreground and background colors
void vga_get_color(uint8_t* fg, uint8_t* bg) {
    if (fg) *fg = vga_color & 0x0F;
    if (bg) *bg = (vga_color >> 4) & 0x0F;
}

// Move cursor to given row and col
void vga_move_cursor(uint8_t row, uint8_t col) {
    if (row >= VGA_HEIGHT) row = VGA_HEIGHT - 1;
    if (col >= VGA_WIDTH) col = VGA_WIDTH - 1;
    cursor_row = row;
    cursor_col = col;
    update_cursor();
}

// Get cursor position
// fuck cursors
void vga_get_cursor(uint8_t* row, uint8_t* col) {
    if (row) *row = cursor_row;
    if (col) *col = cursor_col;
}

// Disable hardware cursor
void vga_disable_cursor() {
    outb(VGA_CMD_PORT, 0x0A);
    outb(VGA_DATA_PORT, 0x20);
}

// Enables hardware cursor with scanline range...probably
void vga_enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(VGA_CMD_PORT, 0x0A);
    outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xC0) | cursor_start);
    outb(VGA_CMD_PORT, 0x0B);
    outb(VGA_DATA_PORT, (inb(VGA_DATA_PORT) & 0xE0) | cursor_end);
}

// Write single character to screen PLUS spechial chars!! uwu
void vga_putc(char c) {
    switch (c) {
        case '\n':
            cursor_col = 0;
            cursor_row++;
            break;
        case '\r':
            cursor_col = 0;
            break;
        case '\t':
            cursor_col = (cursor_col + 4) & ~(4 - 1);  // tab stops every 4 cols
            if (cursor_col >= VGA_WIDTH) {
                cursor_col = 0;
                cursor_row++;
            }
            break;
        default:
            if (c >= ' ') {
                size_t index = cursor_row * VGA_WIDTH + cursor_col;
                video_mem[index] = vga_entry(c, vga_color);
                cursor_col++;
                if (cursor_col >= VGA_WIDTH) {
                    cursor_col = 0;
                    cursor_row++;
                }
            }
            break;
    }

    if (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }

    update_cursor();
}

// Write null-terminated string
void vga_puts(const char* str) {
    if (!str) return;
    while (*str) {
        vga_putc(*str++);
    }
}

// Print 32-bit unsigned integer in hex
void vga_puthex(uint32_t num) {
    const char* hex_chars = "0123456789ABCDEF";
    char buf[9];
    buf[8] = '\0';
    for (int i = 7; i >= 0; i--) {
        buf[i] = hex_chars[num & 0xF];
        num >>= 4;
    }
    vga_puts(buf);
}

// Print 32-bit unsigned integer in decimal
void vga_putdec(uint32_t num) {
    char buf[11]; // enough for 10 digits + null
    int i = 10;
    buf[i] = '\0';

    if (num == 0) {
        vga_putc('0');
        return;
    }

    while (num > 0 && i) {
        buf[--i] = '0' + (num % 10);
        num /= 10;
    }

    vga_puts(&buf[i]);
}
