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

int main(int argc, char* argv[])
{
    int inputFD, outputFD, nread, nwrite, i;
    if (argc != 3) {
        err_sys("error");
    }

    if ((inputFD = open(argv[1], O_RDONLY)) < 0) {
        err_sys("open error");
    }

    if ((outputFD = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, FILE_MODE)) < 0) {
        err_sys("open error");
    }

    while ((nread = read(inputFD, buf, BUFFSIZE)) > 0) {
        for (i = 0; i < nread; ++i) {
            buf[i] = translate(buf[i]);
        }
        if ((nwrite = write(outputFD, buf, nread)) != nread) {
            if (nwrite < 0) {
                err_sys("write error");
            } else {
                err_quit("short write %d / %d", nwrite, nread);
            }
        }
    }
    fsync(outputFD);
    exit(0);
}