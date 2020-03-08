#include "../apue.3e/include/apue.h"

static void pr_ids(char *name)
{
    printf("%s: pid = %ld, ppid = %ld, pgrp = %ld, sid = %ld\n",
            name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)getsid(0));
}

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        pr_ids("child");
        setsid();
        pr_ids("child");
    } else {
        pr_ids("parent");
    }

    exit(0);
}