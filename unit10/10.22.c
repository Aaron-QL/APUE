#include "../apue.3e/include/apue.h"
#include "10.14.c"

static void sig_int(int);

//sigsuspend函数在各阶段的屏蔽字状态
int main(void)
{
    sigset_t new_mask, old_mask, wait_mask;

    pr_mask("program start: ");

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal error");
    }

    sigemptyset(&new_mask);
    sigemptyset(&wait_mask);
    sigaddset(&new_mask, SIGINT);
    sigaddset(&wait_mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        err_sys("sigprocmask error");
    }

    pr_mask("in critical region: ");

    if (sigsuspend(&wait_mask) != -1) {
        err_sys("sigsuspend error");
    }

    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        err_sys("sigprocmask error");
    }

    pr_mask("program exit: ");

    exit(0);
}

static void sig_int(int signo)
{
    pr_mask("\nin sig_int: ");
}
