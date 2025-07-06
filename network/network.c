#include "network.h"
#include "../kernel/kernel.h"
#include "../include/types.h"

static int32_t loopback_packets = 0;

void network_init() {
    loopback_packets = 0;
}

void network_send_loopback(const char *data) {
    loopback_packets++;
    kprint("[LOOPBACK] Packet sent: ");
    kprint(data);
    kprint("\n");
}

int32_t network_get_loopback_count() {
    return loopback_packets;
}
