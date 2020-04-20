#include "../apue.3e/include/apue.h"
#include <ctype.h>
#include <fcntl.h>

//同步I/O demo
#define BUFFSIZE 4096
unsigned char buf[BUFFSIZE];

unsigned char translate(unsigned char c)
{
    if (!isalpha(c)) {
        return c;
    }
    if (c >= 'n') {
        c -= 13;
    } else if (c >= 'a') {
        c += 13;
    } else if (c >= 'N') {
        c -= 13;
    } else {
        c += 13;
    }
    return c;
}

int main(int argc, char *argv[])
{
    int n, nw, input_fd, output_fd;
    if (argc != 3) {
        err_sys("error");
    }

    if ((input_fd = open(argv[1], O_RDONLY)) < 0) {
        err_sys("open error");
    }
    if ((output_fd = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        err_sys("open error");
    }

    while ((n = read(input_fd, buf, BUFFSIZE)) > 0) {
        for (int i = 0; i < n; ++i) {
            buf[i] = translate(buf[i]);
        }
        if ((nw = write(output_fd, buf, n)) != n) {
            if (nw < 0) {
                err_sys("write failed");
            } else {
                err_quit("short write (%d/%d)", nw, n);
            }
        }
    }

    fsync(output_fd);
    exit(0);
}