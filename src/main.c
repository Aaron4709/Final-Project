#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "execute.h"
#include "signals.h"
#include "logger.h"


int main(void) {
  char line[1024];
  while (1) {
  printf("myshell> ");
  fflush(stdout);
  if (!fgets(line, sizeof(line), stdin)) break;
  if (strcmp(line, "\n") == 0) continue;
  char raw[1024];
  strcpy(raw, line);
  struct command *cmd = parse_line(line);
  if (cmd) {
    execute_command(cmd, raw);
    free_command(cmd);
  }
}

return 0;
}
