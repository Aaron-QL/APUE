#include	<signal.h>
#include	<unistd.h>

static void sig_alram(int signo)
{

}

int sleep1(int seconds)
{
    if (signal(SIGALRM, sig_alram) == SIG_ERR) {
        return seconds;
    }

    alarm(seconds);

    pause();

    return alarm(0);
}