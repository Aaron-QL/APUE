#include "../apue.3e/include/apue.h"

int main()
{
    struct stat statbuf;

    if (stat("foo", &statbuf) < 0) {
        err_sys("stat error for foo");
    }

    printf("%d\t %lld \n", statbuf.st_blksize, statbuf.st_blocks);

    exit(0);
}