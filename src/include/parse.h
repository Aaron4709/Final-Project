#ifndef PARSE_H
#define PARSE_H

#define MAX_TOKENS 64

typedef struct {
    char **argv;
    char **argv_pipe;
    char *input_file;
    char *output_file;
    int append;
    int background;
    int has_pipe;
} Command;

Command *parse_line(char *line);
void free_command(Command *cmd);

#endif
