#include "../apue.3e/include/apue.h"
#include <sys/wait.h>

char *init_env[] = {"User=unknown", "PATH=/tmp", NULL};


int main(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        //不带p要使用绝对路径；带p使用相对路径
        //带l表示参数分开排列；带v表示参数放在char *vevtor[]里
        //带e表示指定环境变量，不带e直接继承当前环境变量
        if (execle("/Users/qinlong/c/apue/unit8/echoAll", "arg1", "arg2", "arg3", (char *)0, init_env) < 0) {
            err_sys("execle error");
        }
    }

    if (waitpid(pid, NULL, 0) != pid) {
        err_sys("waitpid error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        char buf[100];
        if (execlp("./echoAll", "arg1", "arg2", "arg3", (char *)0) < 0) {
            err_sys("execlp error");
        }
    }

    exit(0);
}