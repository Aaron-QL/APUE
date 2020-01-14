#include "../apue.3e/include/apue.h"
#include <fcntl.h>

char filebuf1[] = "abcdefghij";
char filebuf2[] = "ABCDEFGHIJ";

int main(void)
{
    int fd;
    if ((fd = creat("file.hole", FILE_MODE)) < 0) {
        err_sys("creat error");
    }

    if (write(fd, filebuf1, 10) != 10) {
        err_sys("write error");
    }

    if (lseek(fd, 16384, SEEK_SET) == -1) {
        err_sys("lseek error");
    }

    if (write(fd, filebuf2, 10) != 10) {
        err_sys("buf2 write error");
    }

    exit(0);
}