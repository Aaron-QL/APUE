#include "../apue.3e/include/apue.h"
#include <fcntl.h>

int main(void)
{
    int fd;

    fd = open("file.hole", O_RDWR | O_TRUNC);

    int i = 0;
    while (i++ < 1000) {
        if (lseek(fd, 3, SEEK_CUR) == -1) {
            err_sys("lseek error");
        }

        if (write(fd, "abc", 3) != 3) {
            err_sys("buf2 write error");
        }

    }

    exit(0);
}