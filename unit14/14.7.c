#include "../apue.3e/include/apue.h"
#include <fcntl.h>
#include "14.5.c"
#include "../unit10/10.24.c"

//父子进程都加记录锁，造成死锁

static void lock_a_byte(char *name, int fd, off_t offset)
{
    if (lock_reg(fd, F_SETLK, F_WRLCK, offset, SEEK_SET, 1) < 0) {
        err_sys("lock_reg error");
    }

    printf("%s: got the lock, byte %lld\n", name, offset);
}

int main(void)
{
    int fd;
    pid_t pid;

    if ((fd = creat("temp.file", FILE_MODE)) < 0) {
        err_sys("open error");
    }

    if (write(fd, "ab", 2) != 2) {
        err_sys("write error");
    }

    TELL_WAIT();
    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if ( pid == 0) {
        lock_a_byte("child", fd, 0);
        TELL_PARENT(getppid());
        WAIT_PARENT();
        lock_a_byte("child", fd, 1);
    } else {
        lock_a_byte("parent", fd, 1);
        TELL_CHILD(pid);
        WAIT_CHILD();
        lock_a_byte("parent", fd, 0);
    }

    close(fd);

    exit(0);
}