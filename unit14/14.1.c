#include "../apue.3e/include/apue.h"
#include <errno.h>
#include <fcntl.h>

//非阻塞I/O

#define BUFFSIZE 4096
char buf[BUFFSIZE];

//设置文件状态
void set_flag(int fd, int flags)
{
    int flag_val;
    if ((flag_val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }

    //设置位
    flag_val |= flags;

    if (fcntl(fd, F_SETFL, flag_val) < 0) {
        err_sys("fcntl error");
    }
}

//移除文件状态
void clr_flag(int fd, int flags)
{
    int flag_val;
    if ((flag_val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }

    //设置位
    flag_val &= ~flags;

    if (fcntl(fd, F_SETFL, flag_val) < 0) {
        err_sys("fcntl error");
    }
}

int main(void)
{
    int n_to_write, n_write;
    char *ptr;

    if ((n_to_write = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) {
        err_sys("read error");
    }

    printf("read %d bytes\n", n_to_write);

    set_flag(STDOUT_FILENO, O_NONBLOCK);

    ptr = buf;
    while (n_to_write > 0) {
        errno = 0;
        n_write = write(STDOUT_FILENO, ptr, n_to_write);
        fprintf(stderr, "nwrite = %d, errno = %d\n", n_write, errno);

        if (n_write > 0) {
            n_to_write -= n_write;
            ptr += n_write;
        }
    }

    clr_flag(STDOUT_FILENO, O_NONBLOCK);
    exit(0);
}