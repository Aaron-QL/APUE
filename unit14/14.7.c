#include "../apue.3e/include/apue.h"
#include <fcntl.h>
#include "14.5.c"
#include "../unit10/10.24.c"

//父子进程都加记录锁，造成死锁

static void lockabyte(char *name, int fd, off_t offset)
{
    if (lock_reg(fd, F_SETLK, F_WRLCK, offset, SEEK_SET, 1) < 0) {
        err_sys("write_lock error");
    }
    printf("%s: got the lock, byte %lld\n", name, (long long) offset);
}

int main(void)
{
    int fd;
    pid_t pid;

    if ((fd = creat("tempfile", FILE_MODE)) < 0) {
        err_sys("creat error");
    }
    if (write(fd, "ab", 2) != 2) {
        err_sys("write error");
    }

    TELL_WAIT();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        lockabyte("child", fd, 1);
        TELL_PARENT(getppid());
        WAIT_PARENT();
        lockabyte("child", fd, 0);
    } else {
        lockabyte("parent", fd, 0);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lockabyte("parent", fd, 1);
    }

    exit(0);
}