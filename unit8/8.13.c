#include "../apue.3e/include/apue.h"
#include <sys/wait.h>

static void print(char *s)
{
    char *ptr = s;
    setbuf(stdout, NULL);

    while (*ptr != 0) {
        putc(*ptr++, stdout);
    }
}

int main(void)
{
    pid_t pid;

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        WAIT_PARENT();
        print("print from child process\n");
    } else {
        TELL_CHILD(pid);
        print("print from parent process\n");
    }

    exit(0);
}