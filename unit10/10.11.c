#include "../apue.3e/include/apue.h"
#include <setjmp.h>

//带时间限制的read操作

static jmp_buf env_alram;

static void sig_alarm(int seconds)
{
    longjmp(env_alram);
}

int main(void) {
    int buffsize = 100;
    int n;
    char buf[n];

    if (signal(SIGALRM, sig_alarm) == SIG_ERR) {
        err_sys("signal error");
    }

    if (setjmp(env_alram) != 0) {
        err_sys("read timeout");
    }

    alarm(10);
    if ((n = read(STDIN_FILENO, buf, buffsize)) < 0) {
        err_sys("read error");
    }
    alarm(0);

    write(STDOUT_FILENO, buf, n);
    exit(0);
}