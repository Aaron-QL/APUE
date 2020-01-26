#include "../apue.3e/include/apue.h"
#include <fcntl.h>


int main(int argc, char **argv)
{
    int fd;
    if ((fd = open(argv[1], O_RDWR)) == -1) {
        err_sys("open error");
    }
    if (lseek(fd, 0, SEEK_SET) == -1) {
        err_sys("seek error");
    }
    char *buf = malloc(sizeof(char) * 100);

    read(fd, buf, 3);
    printf("%s\n", buf);

    if (lseek(fd, 4, SEEK_CUR) == -1) {
        err_sys("seek error");
    }

    write(fd, "zzz", 3);



    read(fd, buf, 3);
    printf("%s\n", buf);

    close(fd);
    return 0;
}