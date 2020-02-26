#include "../apue.3e/include/apue.h"
#include <sys/wait.h>

char *init_env[] = { "USER=unknown", "PATH=/tmp", NULL};

int main(void)
{
    int pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execle("/Users/qinlong/c/apue/unit8/echoAll", "asdfas", "arg1", "asd# E", (char *) 0, init_env) < 0) {
            err_sys("execle error");
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {
        err_sys("waitpid error");
    }


    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        if (execlp("echoAll", "/Users/qinlong/c/apue/unit8/asdfas", "arg1", "asd# E", (char *) 0) < 0) {
            err_sys("execle error");
        }
    }

    exit(0);
}