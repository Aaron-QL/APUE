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
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        print("print from child process\n");
    } else {
        print("print from parent process\n");
    }

    exit(0);
}