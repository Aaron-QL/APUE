#include	<signal.h>
#include	<unistd.h>

static void sig_alrm(int signo)
{
    /* nothing to do, just return to wake up the pause */
}

unsigned int sleep1(unsigned int seconds)
{
    //注册信号处理程序
    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        return seconds;
    }
    //设置闹钟
    alarm(seconds);
    //暂停
    pause();
    //关掉闹钟
    return alarm(0);
}
