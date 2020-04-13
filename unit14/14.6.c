#include "../apue.3e/include/apue.h"
#include <fcntl.h>

pid_t lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
    struct flock lock;

    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    if (fcntl(fd, F_GETLK, &lock) < 0) {
        err_sys("fcntl error");
    }

    return lock.l_pid;
}

int main(void)
{
    printf("pid : %d\n", lock_test(STDIN_FILENO, F_WRLCK, 0, SEEK_SET, 1));
}