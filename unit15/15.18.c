#include "../apue.3e/include/apue.h"

void sig_pipe(int signo)
{
    printf("SIGPIPE caught\n");
    exit(1);
}

int main(void)
{
    pid_t pid;
    char line[MAXLINE];
    int n, pfd1[2], pfd2[2]; //pfd1是父进程->子进程的单向管道，pfd2是子进程->父进程的单向管道

    if (signal(SIGPIPE, sig_pipe) == SIG_ERR) {
        err_sys("signal error");
    }

    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        err_sys("pipe error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        //用标准输入与标准输出的描述符和父进程进行通信，关闭其他描述符
        close(pfd1[1]);
        close(pfd2[0]);
        if (pfd1[0] != STDIN_FILENO) {
            dup2(pfd1[0], STDIN_FILENO);
            close(pfd1[0]);
        }
        if (pfd2[1] != STDOUT_FILENO) {
            dup2(pfd2[1], STDOUT_FILENO);
            close(pfd2[1]);
        }

        if (execl("./add2", "add2", (char *)0) < 0) {
            err_sys("execl error");
        }

        exit(0);
    }

    close(pfd1[0]);
    close(pfd2[1]);

    while (fgets(line, MAXLINE, stdin) != NULL) {
        n = strlen(line);
        if (write(pfd1[1], line, n) != n) {
            err_sys("write error");
        }
        if ((n = read(pfd2[0], line, MAXLINE)) < 0) {
            err_sys("read error");
        }
        if (n == 0) {
            err_msg("child closed pipe");
            break;
        }
        line[n] = 0;
        if (fputs(line, stdout) == EOF) {
            err_sys("fputs error");
        }
    }

    //fgets完要用ferror判断是否发生错误
    if (ferror(stdin)) {
        err_sys("ferror error");
    }
    exit(0);
}