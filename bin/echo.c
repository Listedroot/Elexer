#include "echo.h"
#include "../kernel/kernel.h"

void bin_echo(const char *arg) {
    kprint(arg);
    kprint("\n");
}
