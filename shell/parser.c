#include "parser.h"
#include "../bin/bin.h"
#include "../kernel/kernel.h"
#include "../include/string.h"
#include "../include/types.h"

void shell_parse_and_execute(const char *input) {
    // Only support single-word commands and one argument
    char cmd[32] = {0};
    char arg[96] = {0};
    size_t i = 0, j = 0;
    while (input[i] && input[i] != ' ' && i < 31) {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = 0;
    if (input[i] == ' ') {
        i++;
        while (input[i] && j < 95) {
            arg[j++] = input[i++];
        }
        arg[j] = 0;
    }
    bin_execute(cmd, arg);
}
