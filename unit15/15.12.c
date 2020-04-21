#include "../apue.3e/include/apue.h"

static pid_t *child_ptr = NULL;

static int max_fd;

FILE *popen(const char *cmd, const char *type) {
    int pfd[2];
    pid_t pid;
    FILE *fp;

    //检验参数
    if (cmd == NULL) {
        errno = EINVAL;
        return NULL;
    }
    if (type[0] != 'r' && type[0] != 'w' || type[1] != 0) {
        errno = EINVAL;
        return NULL;
    }

    //创建描述符与进程id的对应关系，在关闭时使用
    if (child_ptr == NULL) {
        max_fd = open_max();
        if ((child_ptr = calloc(max_fd, sizeof(pid_t))) == NULL) {
            err_sys("error");
        }
    }

    //创建管道
    if (pipe(pfd) < 0) {
        err_sys("error");
    }

    //检测描述符
    if (pfd[0] >= max_fd || pfd[1] >= max_fd) {
        close(pfd[0]);
        close(pfd[1]);
        errno = EMFILE;
        return NULL;
    }

    if ((pid = fork()) < 0) {
        return NULL;
    } else if (pid == 0) { //子进程关闭多于描述符
        if (*type == 'r') {
            close(pfd[0]);
            if (pfd[1] != STDOUT_FILENO) {
                dup2(pfd[1], STDOUT_FILENO);
                close(pfd[1]);
            }
        } else {
            close(pfd[1]);
            if (pfd[0] != STDIN_FILENO) {
                dup2(pfd[0], STDOUT_FILENO);
                close(pfd[0]);
            }
        }

        //关闭子进程中映射关系里所有的描述符，因为子进程用不到
        for (int i = 0; i < max_fd; ++i) {
            if (child_ptr[i] > 0) {
                close(i);
            }
        }

        execl("/bin/sh", "sh", "-c", (char *)0);
        _exit(127);
    }

    //父进程关闭多于描述符，并获取描述符对应文件指针
    if (*type == 'r') {
        close(pfd[1]);
        if ((fp = fdopen(pfd[0], type)) == NULL) {
            return NULL;
        }
    } else {
        close(pfd[0]);
        if ((fp = fdopen(pfd[1], type)) == NULL) {
            return NULL;
        }
    }

    //将描述符与子进程id绑定，为了在关闭时通过描述符能找到子进程ID
    child_ptr[fileno(fp)] = pid;
    return fp;
}

int pclose(FILE *fp)
{
    int fd, status;
    pid_t pid;
    if (fp == NULL || //参数错误
        child_ptr == NULL || //子进程指针未初始化
        (fd = fileno(fp)) < 0 || //文件指针获取不到对应的描述符
        (pid = child_ptr[fd]) == 0) // 找不到与该描述符对应的进程ID
    {
        errno = EINVAL;
        return -1;
    }

    child_ptr[fd] = 0; //解除描述符与子进程ID的绑定关系
    if (fclose(fp) == EOF) { //关闭管道文件
        return -1;
    }

    while (waitpid(pid, &status, 0) < 0) { //等待失败有一种情况时被中断的系统调用，这种情况需要重新等待
        if (errno != EINTR) {
            return -1;
        }
    }

    return status;
}