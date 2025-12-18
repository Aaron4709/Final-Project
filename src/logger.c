#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <logger.h>

static int log_fd = -1;

void init_logger(void) {
    log_fd = open("myshell.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
}

void log_command(pid_t pid, char *cmd, int status) {
    char buffer[256];
    int code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;

    int len = snprintf(buffer, sizeof(buffer),
        "[pid=%d] cmd=\"%s\" status=%d\n",
        pid, cmd, code);

    write(log_fd, buffer, len);
}

void close_logger(void) {
    close(log_fd);
}
