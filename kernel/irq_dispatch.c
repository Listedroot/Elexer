#include <stdint.h>
#include "../drivers/keyboard.h"
#include "../drivers/timer.h"
#include "../drivers/serial.h"
#include "interrupts.h"

void irq_handler_common(uint8_t irq) {
    // Acknowledge the interrupt
    pic_send_eoi(irq);
    
    // Handle the specific IRQ
    switch (irq) {
        case 0:  // Timer interrupt
            timer_handler();
            break;
            
        case 1:  // Keyboard interrupt
            keyboard_handler();
            break;
            
        case 4:  // COM1 IRQ
            // Could add serial_handler() here if implemented IRQ-driven
            break;
            
        default:
            // Unhandled IRQ - log it
            serial_write_string(SERIAL_COM1_BASE, "Unhandled IRQ: ");
            serial_write_byte(SERIAL_COM1_BASE, '0' + (irq / 10));
            serial_write_byte(SERIAL_COM1_BASE, '0' + (irq % 10));
            serial_write_string(SERIAL_COM1_BASE, "\n");
            break;
    }
}
