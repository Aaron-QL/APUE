#include "../apue.3e/include/apue.h"

int main(void)
{
    int pipe_fd[2];
    pid_t pid;
    int n;
    const int buff_size = 4096;
    char buf[buff_size];

    if (pipe(pipe_fd) < 0) {
        err_sys("error");
    }

    if ((pid = fork()) < 0) {
        err_sys("error");
    } else if (pid > 0) {
        close(pipe_fd[0]);
        write(pipe_fd[1], "hello world!\n", 13);
    } else {
        close(pipe_fd[1]);
        if ((n = read(pipe_fd[0], buf, buff_size)) < 0) {
            err_sys("error");
        }
        write(STDOUT_FILENO, buf, n);
    }

    exit(0);
}