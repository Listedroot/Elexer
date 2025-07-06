#include "shell.h"
#include "../kernel/kernel.h"
#include "../bin/bin.h"
#include "parser.h"
#include "../include/types.h"
#include "../include/string.h"
#include "../include/port_io.h"

static char input[INPUT_BUF];
static char history[MAX_HISTORY][INPUT_BUF];
static int32_t history_count = 0;
static int32_t history_pos = -1;
static int32_t cursor_pos = 0;

static void clear_line() {
    kprint("\r");
    for (int32_t i = 0; i < 80; i++) kprint(" ");
    kprint("\r");
}

void shell_add_to_history(const char *cmd) {
    if (history_count > 0 && strcmp(history[0], cmd) == 0) return;
    
    for (int32_t i = MAX_HISTORY - 1; i > 0; i--) {
        strcpy(history[i], history[i-1]);
    }
    strncpy(history[0], cmd, INPUT_BUF - 1);
    history[0][INPUT_BUF - 1] = '\0';
    
    if (history_count < MAX_HISTORY) history_count++;
    history_pos = -1;
}

const char *shell_get_history(int index) {
    if (index < 0 || index >= history_count) return NULL;
    return history[index];
}

int shell_history_count() {
    return history_count;
}

void shell_init() {
    kprint("\nEnhanced Shell v0.1\n");
    kprint("Type 'help' for available commands\n");
    for (int32_t i = 0; i < MAX_HISTORY; i++) {
        history[i][0] = '\0';
    }
}

int shell_readline(char *buf, int maxlen);

void shell_run() {
    kprint("$ ");
    int len = shell_readline(input, INPUT_BUF);
    if (len > 0) {
        shell_add_to_history(input);
        shell_parse_and_execute(input);
    }
}

void handle_arrow_keys(unsigned char scancode, char *buf, int *pos, int maxlen) {
    if (scancode == 0x48) { // Up arrow
        if (history_pos < history_count - 1) {
            history_pos++;
            clear_line();
            kprint("$ ");
            const char *hist = shell_get_history(history_pos);
            strncpy(buf, hist, maxlen - 1);
            *pos = strlen(hist);
            kprint(buf);
        }
    } else if (scancode == 0x50) { // Down arrow
        if (history_pos > 0) {
            history_pos--;
            clear_line();
            kprint("$ ");
            const char *hist = shell_get_history(history_pos);
            strncpy(buf, hist, maxlen - 1);
            *pos = strlen(hist);
            kprint(buf);
        } else if (history_pos == 0) {
            history_pos--;
            clear_line();
            kprint("$ ");
            buf[0] = '\0';
            *pos = 0;
        }
    } else if (scancode == 0x4B) { // Left arrow
        if (*pos > 0) {
            (*pos)--;
            kprint("\b");
        }
    } else if (scancode == 0x4D) { // Right arrow
        if (*pos < (int)strlen(buf)) {
            kprint(&buf[*pos]);
            (*pos)++;
        }
    }
}

int shell_readline(char *buf, int maxlen) {
    int i = 0;
    buf[0] = '\0';
    cursor_pos = 0;
    
    while (1) {
        unsigned char scancode = 0;
        while ((scancode = inb(0x64)) & 1 == 0);
        scancode = inb(0x60);
        
        if (scancode == 0xE0) {
            scancode = inb(0x60);
            handle_arrow_keys(scancode, buf, &cursor_pos, maxlen);
            continue;
        }
        
        if (scancode == 0x0F) { // Tab
            // Simple tab completion - just add spaces for now
            if (i + 4 < maxlen) {
                for (int j = 0; j < 4; j++) {
                    buf[i++] = ' ';
                    kprint(" ");
                }
                cursor_pos = i;
            }
            continue;
        }
        
        static const char map[128] = {
            0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
            '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
            'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
            'z','x','c','v','b','n','m',',','.','/',0,0,0,0,0,0,0,
            ' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        
        if (scancode < 128) {
            char c = map[scancode];
            if (c == '\r' || c == '\n') {
                buf[i] = 0;
                kprint("\n");
                return i;
            } else if (c == '\b') {
                if (i > 0 && cursor_pos > 0) {
                    for (int j = cursor_pos - 1; j < i; j++) {
                        buf[j] = buf[j+1];
                    }
                    i--;
                    cursor_pos--;
                    clear_line();
                    kprint("$ ");
                    kprint(buf);
                    for (int j = cursor_pos; j < i; j++) {
                        kprint("\b");
                    }
                }
            } else if (c >= 32 && c < 127) {
                if (i < maxlen - 1) {
                    for (int j = i; j > cursor_pos; j--) {
                        buf[j] = buf[j-1];
                    }
                    buf[cursor_pos] = c;
                    i++;
                    cursor_pos++;
                    clear_line();
                    kprint("$ ");
                    kprint(buf);
                    for (int j = cursor_pos; j < i; j++) {
                        kprint("\b");
                    }
                }
            }
        }
    }
}

char shell_getchar() {
    while (1) {
        unsigned char scancode = 0;
        while ((scancode = inb(0x64)) & 1 == 0);
        scancode = inb(0x60);
        
        static const char map[128] = {
            0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b',
            '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
            'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
            'z','x','c','v','b','n','m',',','.','/',0,0,0,0,0,0,0,
            ' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        
        if (scancode < 128) {
            char c = map[scancode];
            if (c) return c;
        }
    }
}
