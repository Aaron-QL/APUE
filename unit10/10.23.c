#include "../apue.3e/include/apue.h"

volatile sig_atomic_t quit_flag = 0;


static void sig_int(int signo)
{
    if (signo == SIGINT) {
        printf("\ninterrupt\n");
    } else if (signo == SIGQUIT) {
        quit_flag = 1;
    }
}

int main(void)
{
    sigset_t new_mask, old_mask, zero_mask;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        err_sys("signal error");
    }
    if (signal(SIGQUIT, sig_int) == SIG_ERR) {
        err_sys("signal error");
    }

    sigemptyset(&new_mask);
    sigemptyset(&zero_mask);
    sigaddset(&new_mask, SIGQUIT);

    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        err_sys("sigprocmask error");
    }

    //仅当捕捉到退出信号时唤醒主例程
    while (quit_flag == 0) {
        sigsuspend(&zero_mask);
    }

    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        err_sys("sigprocmask error");
    }

    quit_flag = 0;
    exit(0);

}