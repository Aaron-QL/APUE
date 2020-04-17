#include "../apue.3e/include/apue.h"
#include <signal.h>

#define	SIGBAD(signo)	((signo) <= 0 || (signo) >= NSIG)

//实现信号集的设置

int sigemptyset(sigset_t *ptr)
{
    *ptr = 0;
    return 0;
}

int sigfillset(sigset_t *ptr)
{
    *ptr = ~ (sigset) 0;
    return 0;
}

//开启一位
int sigaddset(sigset *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }
    *set |= 1 << (signo - 1);
    return 0;
}

//关闭一位
int sigdelset(sigset *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }

    *set &= ~(1 << (signo - 1));
}

int sigismember(const sigset_t *set, int signo)
{
    if (SIGBAD(signo)) {
        errno = EINVAL;
        return -1;
    }

    return (*set & (1 << (signo - 1))) != 0;
}