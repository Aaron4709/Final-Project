#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "parse.h"
#include "execute.h"
#include "signals.h"
#include "logger.h"

int main(void) {
    char line[1024];

    setup_signals();
    init_logger();

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            break; // EOF
        }

        // ignore empty line
        if (strcmp(line, "\n") == 0) {
            continue;
        }

        Command *cmd = parse_line(line);
        if (cmd == NULL) {
            continue;
        }

        if (execute_command(cmd, line) == -1) {
            free_command(cmd);
            break; // exit or quit
        }

        free_command(cmd);
    }

    close_logger();
    return 0;
}
