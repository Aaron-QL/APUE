#include "../apue.3e/include/apue.h"
#include <errno.h>

static void sig_hup()
{
    printf("SIGHUP received, pid = %ld\n", (long)getpid());
    fflush(stdout);
}

static void sig_cont()
{
    printf("SIGCON received, pid = %ld\n", (long)getpid());
    fflush(stdout);
}

static void pr_ids(char *name)
{
    printf("%s: pd = %ld, ppid = %ld, pgrp = %ld, tpgrp = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

int main(void) {
    char c;
    pid_t pid;
    pr_ids("parent");
    if ((pid = fork()) < 0) {
        err_sys("fork_error");
    } else if (pid > 0) {
        sleep(5);
    } else {
        pr_ids("child");
        signal(SIGHUP, sig_hup);
        signal(SIGCONT, sig_cont);
        kill(getpid(), SIGTSTP);
        pr_ids("child");
        if (read(STDIN_FILENO, &c, 1) != 1) {
            err_sys("read error %d on controlling TTY", errno);
        }
    }

    exit(0);
}