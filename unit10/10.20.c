#include "../apue.3e/include/apue.h"
#include "10.14.c"
#include <setjmp.h>
#include <time.h>

//sigjmp 恢复屏蔽字
static void sig_usr1(int);
static void sig_alarm(int);

static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;

int main(void)
{
    if (signal(SIGUSR1, sig_usr1) == SIG_ERR) {
        err_sys("signal error");
    }

    if (signal(SIGALRM, sig_alarm) == SIG_ERR) {
        err_sys("signal error");
    }

    pr_mask("starting main: ");

    if (sigsetjmp(jmpbuf, 1) != 0) {
        pr_mask("ending main: ");
        exit(0);
    }

    canjump = 1;

    for (;;) {
        pause();
    }
}

static void sig_usr1(int signo)
{
    time_t starttime;

    if (canjump == 0) {
        return;
    }

    pr_mask("starting sig_usr1: ");

    alarm(3);
    starttime = time(NULL);
    for ( ; ; ) {
        /* busy wait for 5 seconds */
        if (time(NULL) > starttime + 5) {
            break;
        }
    }

    pr_mask("finishing sig_usr1: ");

    canjump = 0;
    siglongjmp(jmpbuf, 1);	/* jump back to main, don't return */
}

static void sig_alarm(int signo)
{
    pr_mask("in sig_alrm: ");
}
