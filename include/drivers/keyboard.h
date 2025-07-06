#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

// Key codes (non-printable keys)
enum key_code {
    KEY_NONE = 0,
    KEY_ESCAPE = 1,
    KEY_ENTER = 28,
    KEY_LEFT_CTRL = 29,
    KEY_LEFT_SHIFT = 42,
    KEY_RIGHT_SHIFT = 54,
    KEY_ALT = 56,
    KEY_SPACE = 57,
    KEY_CAPS_LOCK = 58,
    KEY_F1 = 59,
    KEY_F2 = 60,
    KEY_F3 = 61,
    KEY_F4 = 62,
    KEY_F5 = 63,
    KEY_F6 = 64,
    KEY_F7 = 65,
    KEY_F8 = 66,
    KEY_F9 = 67,
    KEY_F10 = 68,
    KEY_NUM_LOCK = 69,
    KEY_SCROLL_LOCK = 70,
    KEY_HOME = 71,
    KEY_UP = 72,
    KEY_PAGE_UP = 73,
    KEY_LEFT = 75,
    KEY_RIGHT = 77,
    KEY_END = 79,
    KEY_DOWN = 80,
    KEY_PAGE_DOWN = 81,
    KEY_INSERT = 82,
    KEY_DELETE = 83,
    KEY_F11 = 87,
    KEY_F12 = 88,
    KEY_RIGHT_CTRL = 91,
    KEY_RIGHT_ALT = 92,
    KEY_LEFT_GUI = 93,
    KEY_RIGHT_GUI = 94,
    KEY_MENU = 101
};

// Keyboard state structure
typedef struct {
    bool left_shift : 1;
    bool right_shift : 1;
    bool caps_lock : 1;
    bool num_lock : 1;
    bool scroll_lock : 1;
    bool left_ctrl : 1;
    bool right_ctrl : 1;
    bool left_alt : 1;
    bool right_alt : 1;
    bool left_gui : 1;
    bool right_gui : 1;
} keyboard_state_t;

// Initialize the keyboard driver
void keyboard_init(void);

// Get the current keyboard state
const keyboard_state_t* keyboard_get_state(void);

// Check if a key is currently pressed
bool keyboard_is_pressed(enum key_code key);

// Get a single character from the keyboard (non-blocking)
// Returns 0 if no key is available
char keyboard_get_char(void);

// Wait for and get a single character from the keyboard (blocking)
char keyboard_wait_char(void);

// Set the keyboard LED states
void keyboard_set_leds(bool scroll_lock, bool num_lock, bool caps_lock);

#endif // KEYBOARD_H
