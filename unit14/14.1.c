#include "../apue.3e/include/apue.h"
#include <errno.h>
#include <fcntl.h>

#define BUFFSIZE 4096
char buf[BUFFSIZE];

//非阻塞I/O
void set_flag(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }

    //设置位
    val |= flags;
    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("fcntl error");
    }
}

void clr_flag(int fd, int flags)
{
    int val;
    if ((val = fcntl(fd, F_GETFL, 0)) < 0) {
        err_sys("fcntl error");
    }

    //屏蔽位
    val &= ~flags;

    if (fcntl(fd, F_SETFL, val) < 0) {
        err_sys("fcntl error");
    }
}

int main(void)
{
    int ntowrite, nwrite;
    char *ptr;

    if ((ntowrite = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) {
        err_sys("read error");
    }
    printf("read %d bytes\n", ntowrite);

    set_flag(STDOUT_FILENO, O_NONBLOCK);

    ptr = buf;
    while (ntowrite > 0) {
        nwrite = write(STDOUT_FILENO, ptr, ntowrite);
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);
        if (nwrite > 0) {
            ntowrite -= nwrite;
            ptr += nwrite;
        }
    }

    clr_flag(STDOUT_FILENO, O_NONBLOCK);
    exit(0);
}