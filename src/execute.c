#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "execute.h"
#include "builtin.h"
#include "logger.h"

static void handle_redirection(Command *cmd) {
    int fd;

    if (cmd->input_file) {
        fd = open(cmd->input_file, O_RDONLY);
        if (fd < 0) {
            perror("input redirection");
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if (cmd->output_file) {
        if (cmd->append)
            fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else
            fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

        if (fd < 0) {
            perror("output redirection");
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
}

int execute_command(Command *cmd, char *raw) {
    int builtin = handle_builtin(cmd->argv);
    if (builtin == -1) return -1;
    if (builtin == 1) return 0;

    if (cmd->has_pipe) {
        int fd[2];
        pipe(fd);

        pid_t left = fork();
        if (left == 0) {
            signal(SIGINT, SIG_DFL);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            handle_redirection(cmd);
            execvp(cmd->argv[0], cmd->argv);
            perror("execvp");
            exit(1);
        }

        pid_t right = fork();
        if (right == 0) {
            signal(SIGINT, SIG_DFL);
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            close(fd[0]);
            execvp(cmd->argv_pipe[0], cmd->argv_pipe);
            perror("execvp");
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);
        waitpid(left, NULL, 0);
        waitpid(right, NULL, 0);
        return 0;
    }

    pid_t pid = fork();
    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        handle_redirection(cmd);
        execvp(cmd->argv[0], cmd->argv);
        perror("execvp");
        exit(1);
    }

    if (!cmd->background) {
        int status;
        waitpid(pid, &status, 0);
        log_command(pid, raw, status);
    } else {
        printf("[bg] started pid %d\n", pid);
    }

    return 0;
}
