#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

static char *dup_token(const char *s) {
    char *p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

Command *parse_line(char *line) {
    Command *cmd = malloc(sizeof(Command));
    int i = 0;

    if (!cmd) return NULL;

    cmd->argv = malloc(sizeof(char *) * MAX_TOKENS);
    cmd->argv_pipe = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = 0;
    cmd->background = 0;
    cmd->has_pipe = 0;

    if (!cmd->argv) {
        free(cmd);
        return NULL;
    }

    char *token = strtok(line, " \t\n");
    while (token != NULL) {
        if (strcmp(token, "&") == 0) {
            cmd->background = 1;
        }
        else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) cmd->input_file = dup_token(token);
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd->output_file = dup_token(token);
                cmd->append = 0;
            }
        }
        else if (strcmp(token, ">>") == 0) {
            token = strtok(NULL, " \t\n");
            if (token) {
                cmd->output_file = dup_token(token);
                cmd->append = 1;
            }
        }
        else if (strcmp(token, "|") == 0) {
            cmd->has_pipe = 1;
            cmd->argv[i] = NULL;

            cmd->argv_pipe = malloc(sizeof(char *) * MAX_TOKENS);
            int j = 0;
            token = strtok(NULL, " \t\n");
            while (token != NULL) {
                cmd->argv_pipe[j++] = dup_token(token);
                token = strtok(NULL, " \t\n");
            }
            cmd->argv_pipe[j] = NULL;
            return cmd;
        }
        else {
            cmd->argv[i++] = dup_token(token);
        }
        token = strtok(NULL, " \t\n");
    }

    cmd->argv[i] = NULL;
    return cmd;
}

void free_command(Command *cmd) {
    int i;

    if (!cmd) return;

    if (cmd->argv) {
        for (i = 0; cmd->argv[i]; i++) {
            free(cmd->argv[i]);
        }
        free(cmd->argv);
    }

    if (cmd->argv_pipe) {
        for (i = 0; cmd->argv_pipe[i]; i++) {
            free(cmd->argv_pipe[i]);
        }
        free(cmd->argv_pipe);
    }

    free(cmd->input_file);
    free(cmd->output_file);
    free(cmd);
}
