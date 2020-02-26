#include "../apue.3e/include/apue.h"
#include <sys/wait.h>

void print(char *s)
{
    setbuf(stdout, NULL);
    char *ptr;
    int c;
    for (ptr = s; *ptr != 0; ptr++) {
        c = *ptr;
        putc(c, stdout);
    }
}

int main(void) {
    int pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        print("print from child process\n");
    } else {
        print("print from parent process\n");
    }

    exit(0);
}