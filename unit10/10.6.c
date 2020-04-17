#include "../apue.3e/include/apue.h"
#include <signal.h>

static void sig_child(int signo);

int main(void)
{
    pid_t pid;

    if (signal(SIGCHLD, sig_child) == SIG_ERR) {
        err_sys("signal error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        sleep(1);
        exit(0);
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        sleep(2);
        exit(1);
    }

    sleep(3);
    exit(0);
}

static void sig_child(int signo)
{
    pid_t pid;
    int status;

    //有需要等待的子进程时，先回收子进程，避免重复的信号发生
    if ((pid = wait(&status)) < 0) {
        err_sys("wait error");
    }
    //之后立即重置信号处理程序
    if (signal(SIGCHLD, sig_child) == SIG_ERR) {
        err_sys("signal error");
    }

    printf("received signal: %d, pid: %d, status: %d\n", signo, pid, status);
}