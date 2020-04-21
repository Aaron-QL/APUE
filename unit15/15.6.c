#include "../apue.3e/include/apue.h"
#include <sys/wait.h>

#define DEFAULT_PAGER "/usr/bin/more"

int main(int argc, char *argv[]) {
    pid_t pid;
    int n;
    int pipe_fd[2];
    char *pager, *argv0;
    FILE *file_ptr;
    char line[MAXLINE];

    if (argc != 2) {
        err_sys("error");
    }

    if ((file_ptr = fopen(argv[1], "r")) == NULL) {
        err_sys("fopen error");
    }

    if (pipe(pipe_fd) < 0) {
        err_sys("error");
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        close(pipe_fd[0]);

        while (fgets(line, MAXLINE, file_ptr) != NULL) {
            n = strlen(line);
            if (write(pipe_fd[1], line, n) != n) {
                err_sys("write error to pipe");
            }
        }
        if (ferror(file_ptr)) {
            err_sys("fgets error");
        }
        close(pipe_fd[1]);
        if (waitpid(pid, NULL, 0) < 0) {
            err_sys("waitpid error");
        }
        exit(0);
    } else {
        close(pipe_fd[1]);

        if (dup2(pipe_fd[0], STDIN_FILENO) != STDIN_FILENO) {
            err_sys("dup2 error to stdin");
        }
        close(pipe_fd[0]);

        if ((pager = getenv("PAGER")) ==NULL) {
            pager = DEFAULT_PAGER;
        }

        if ((argv0 = strrchr(pager, '/')) != NULL) {
            argv0++;
        } else {
            argv0 = pager;
        }

        if (execl(pager, argv0, (char *)0) < 0) {
            err_sys("execl error for %s", pager);
        }
    }

    exit(0);
}


