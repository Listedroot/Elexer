#ifndef SHELL_H
#define SHELL_H

#define MAX_HISTORY 10
#define INPUT_BUF 128

void shell_init();
void shell_run();
void shell_add_to_history(const char *cmd);
const char *shell_get_history(int index);
int shell_history_count();
int shell_readline(char *buf, int maxlen);

#endif
