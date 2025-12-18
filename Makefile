CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude

SRC = src/main.c src/parse.c src/execute.c src/builtin.c src/signals.c src/logger.c
OBJ = $(SRC:.c=.o)

myshell: $(OBJ)
	$(CC) $(CFLAGS) -o myshell $(OBJ)

clean:
	rm -f $(OBJ) myshell
