#include "../apue.3e/include/apue.h"

//使用信号实现父、子进程同步
static volatile sig_atomic_t sig_flag;

static sigset_t new_mask, old_mask, zero_mask;

static void sig_usr(int signo)
{
    sig_flag = 1;
}

void TELL_WAIT(void)
{
    sigemptyset(&new_mask);
    sigemptyset(&zero_mask);
    sigaddset(&new_mask, SIGUSR1);
    sigaddset(&new_mask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        err_sys("sigprocmask error");
    }
}

void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR1);
}

void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void)
{
    while (sig_flag == 0) {
        sigsuspend(&zero_mask);
    }

    sig_flag = 0;
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        err_sys("sigprocmask error");
    }
}

void WAIT_CHILD(void)
{
    while (sig_flag == 0) {
        sigsuspend(&zero_mask);
    }

    sig_flag = 0;
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        err_sys("sigprocmask error");
    }
}