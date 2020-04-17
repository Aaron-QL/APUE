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
    switch (signo) {
        case SIGUSR1:
            printf("received SIGUSR1\n");
            break;
        case SIGUSR2:
            printf("received SIGUSR2\n");
            break;
        default:
            err_dump("received signal %d\n", signo);
    }
}