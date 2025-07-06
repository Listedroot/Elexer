#include "network.h"
#include "../kernel/kernel.h"
#include "../include/string.h"
#include "../include/types.h"

static void itoa(int32_t n, char *buf, int32_t base);

extern int32_t network_get_loopback_count();

void netstat() {
    kprint("Loopback packets: ");
    char buf[16];
    itoa(network_get_loopback_count(), buf, 10);
    kprint(buf);
    kprint("\n");
}

static void itoa(int32_t n, char *buf, int32_t base) {
    char tmp[16];
    int32_t i = 0, j = 0;
    if (n == 0) { 
        buf[0] = '0'; 
        buf[1] = 0; 
        return; 
    }
    while (n > 0) { 
        tmp[i++] = '0' + (n % base); 
        n /= base; 
    }
    while (i > 0) 
        buf[j++] = tmp[--i];
    buf[j] = 0;
}
