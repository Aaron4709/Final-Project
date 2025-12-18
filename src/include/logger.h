#ifndef LOGGER_H
#define LOGGER_H

#include <sys/types.h>

void init_logger(void);
void log_command(pid_t pid, char *cmd, int status);
void close_logger(void);

#endif
