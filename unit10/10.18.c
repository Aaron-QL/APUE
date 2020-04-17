#include "../apue.3e/include/apue.h"


//使用sigaction实现signal
union __sigaction_u *my_signal(int signo, union __sigaction_u *func)
{
    //声明sigaction结构
    struct sigaction new_act, old_act;

    //设置捕捉函数
    new_act.__sigaction_u = func;

    //设置屏蔽字
    sigemptyset(&new_act.sa_mask);
    new_act.sa_flags = 0;
    if (signo == SIGALRM) {
#ifdef	SA_INTERRUPT
        new_act.sa_falgs |= SA_INTERRUPT;
#endif
    } else {
        new_act.sa_flags |= SA_RESTART;
    }

    if (sigaction(signo, &new_act, &old_act) < 0) {
        err_sys("sigaction error");
    }

    return old_act.__sigaction_u;
}