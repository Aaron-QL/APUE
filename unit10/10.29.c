#include "../apue.3e/include/apue.h"

void sig_alarm(int signo)
{

}

int my_sleep(int seconds)
{
    unsigned int unslept;
    sigset_t new_mask, old_mask, suspend_mask;
    struct sigaction new_action, old_action;

    // 设置闹钟信号的动作
    new_action.__sigaction_u = (union __sigaction_u) sig_alarm;
    new_action.sa_flags = 0;
    sigemptyset(&new_action.sa_mask);
    if (sigaction(SIGALRM, &new_action, &old_action) < 0) {
        return -1;
    }

    //屏蔽字增加闹钟信号
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    if (sigprocmask(SIG_BLOCK, &new_mask, &old_mask) < 0) {
        return -1;
    }

    //执行alram
    alarm(seconds);

    //从旧屏蔽字里删除SIGARLAM，确保这个信号不被屏蔽
    suspend_mask = old_mask;
    sigdelset(&suspend_mask, SIGALRM);

    //挂起等待ALARM信号
    sigsuspend(&suspend_mask);

    //捕捉到信号后，清除闹钟并记录返回值
    unslept = alarm(0);

    //恢复信号捕捉函数
    if (sigaction(SIGALRM, &old_action, NULL) < 0) {
        return -1;
    }

    //恢复屏蔽字
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) < 0) {
        return -1;
    }

    //返回解除alram时的返回值
    return unslept;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        exit(1);
    }

    int seconds = atoi(argv[1]);

    my_sleep(seconds);
}