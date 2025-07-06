#include "bin.h"
#include "../kernel/kernel.h"
#include "echo.h"
#include "help.h"
#include "../network/network.h"
#include "../include/string.h"
#include "../include/types.h"

void bin_execute(const char *cmd, const char *arg) {
    if (!cmd || !*cmd) return;
    if (strcmp(cmd, "echo") == 0) {
        bin_echo(arg);
    } else if (strcmp(cmd, "help") == 0) {
        bin_help();
    } else if (strcmp(cmd, "netstat") == 0) {
        netstat();
    } else {
        kprint("Unknown command.\n");
    }
}

// Minimal str_equal for kernel
int str_equal(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return *a == *b;
}
