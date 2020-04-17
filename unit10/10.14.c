#include "../apue.3e/include/apue.h"
#include <errno.h>

void pr_mask(const char *str)
{
    int errno_save = errno;
    sigset_t sigset;

    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_sys("sigprocmask error");
    }

    printf("%s ", str);
    if (sigismember(&sigset, SIGINT)) {
        printf(" SIGINT");
    }

    if (sigismember(&sigset, SIGCHLD)) {
        printf(" SIGCHLD");
    }

    if (sigismember(&sigset, SIGHUP)) {
        printf(" SIGHUP");
    }

    if (sigismember(&sigset, SIGTTIN)) {
        printf(" SIGTTIN");
    }

    if (sigismember(&sigset, SIGILL)) {
        printf(" SIGILL");
    }


    if (sigismember(&sigset, SIGTSTP)) {
        printf(" SIGTSTP");
    }


    if (sigismember(&sigset, SIGCONT)) {
        printf(" SIGCONT");
    }


    if (sigismember(&sigset, SIGABRT)) {
        printf(" SIGABRT");
    }

    if (sigismember(&sigset, SIGQUIT)) {
        printf(" SIGQUIT");
    }
    if (sigismember(&sigset, SIGUSR1)) {
        printf(" SIGUSR1");
    }
    if (sigismember(&sigset, SIGALRM)) {
        printf(" SIGALRM");
    }
    printf("\n");
    errno = errno_save;
}