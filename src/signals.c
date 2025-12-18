#include <signal.h>
#include <sys/wait.h>

void sigchld_handler(int sig) {
    (void)sig;

    int status;
    while (waitpid(-1, &status, WNOHANG) > 0) {
    }
}

void setup_signals(void) {
    signal(SIGINT, SIG_IGN);
    signal(SIGCHLD, sigchld_handler);
}
