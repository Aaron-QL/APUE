#include "../apue.3e/include/apue.h"

static void sig_quit(int signo);

int main(void)
{
    sigset_t new_mask, old_mask, pend_mask;

    //1. 设置退出信号的处理程序
    if (signal(SIGQUIT, sig_quit) == SIG_ERR) {
        err_sys("signal error");
    }

    //2. 屏蔽退出信号,并保留旧的屏蔽字
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGQUIT);
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        err_sys("sigprocmask error");
    }

    //3. 睡眠5秒
    sleep(5);

    //4. 检测是否有未决的屏蔽信号
    if (sigpending(&pend_mask) < 0) {
        err_sys("sigpending error");
    }
    if (sigismember(&pend_mask, SIGQUIT)) {
        printf("\nSIGQUIT pending\n");
    }

    //5. 恢复屏蔽字
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        err_sys("sigprocmask error");
    }
    printf("SIGQUIT unblocked\n");

    //6. 睡眠5秒
    sleep(5);
    exit(0);
}

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) {
        err_sys("can't reset SIGQUIT");
    }
}