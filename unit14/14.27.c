#include "../apue.3e/include/apue.h"
#include <fcntl.h>
#include <sys/mman.h>

#define COPY_INCR 1024*1024*1024

int main(int argc, char *argv[])
{
    int fd_in, fd_out;
    void *src, *dst;
    size_t copy_size;
    struct stat stat_buf;
    off_t copy_index = 0;

    if (argc != 3) {
        err_sys("argc error");
    }

    if ((fd_in = open(argv[1], O_RDONLY)) < 0) {
        err_sys("open error");
    }

    if ((fd_out = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        err_sys("open error");
    }

    if (fstat(fd_in, &stat_buf) < 0) {
        err_sys("fstat error");
    }

    if (ftruncate(fd_out, stat_buf.st_size) < 0) {
        err_sys("ftruncate error");
    }

    while (copy_index < stat_buf.st_size) {
        if (stat_buf.st_size - copy_index > COPY_INCR) {
            copy_size = COPY_INCR;
        } else {
            copy_size = stat_buf.st_size - copy_index;
        }

        if ((src = mmap(0, copy_size, PROT_READ, MAP_SHARED, fd_in, copy_index)) == MAP_FAILED) {
            err_sys("mmap error");
        }

        if ((dst = mmap(0, copy_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd_out, copy_index)) == MAP_FAILED) {
            err_sys("mmap error");
        }

        memcpy(dst, src, copy_size);
        munmap(src, copy_size);
        munmap(dst, copy_size);
        copy_index += copy_size;
    }
    exit(0);
}

