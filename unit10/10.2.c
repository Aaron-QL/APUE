#include "../apue.3e/include/apue.h"

static void sig_usr(int signo);

int main(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_sys("signal error");
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        err_sys("signal error");
    }

    for (;;) {
        pause();
    }
}

static void sig_usr(int signo)
{
    if (signo == SIGUSR1) {
        printf("received SIGUSR1\n");
    } else if (signo == SIGUSR2) {
        printf("received SIGUSR2\n");
    } else {
        err_dump("received signal %d\n", signo);
    }
}