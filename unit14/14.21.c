#include "../apue.3e/include/apue.h"
#include <ctype.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

#define BUFFSIZE 4096
#define NBUF 8

enum rwop {
    UNUSED = 0;
    READ_PNEDING = 1;
    WRITE_PENDING = 2;
};

struct buf {
    enum rwop op;
    int last;
    struct aiocb aiocb;
    unsigned char data[BUFFSIZE];
};

struct buf bufs[NBUF];

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
    struct stat stat_buf;
    int input_fd, output_fd, off;
    const struct aiocb *aio_list[NBUF];

    if (argc != 3) {
        err_sys("error");
    }

    if ((input_fd = open(argv[1], O_RDONLY)) < 0) {
        err_sys("error");
    }
    if ((output_fd = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, FILE_MODE)) < 0) {
        err_sys("error");
    }
    if (fstat(input_fd, &stat_buf) < 0) {
        err_sys("error");
    }

    for (int i = 0; i < NBUF; ++i) {
        bufs[i].op = UNUSED;
        bufs[i].aiocb.aio_buf = bufs[i].data;
        bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
        aio_list[i] = NULL;
    }

    numop = 0;
    for (;;) {
        for (int i = 0; i < NBUF; ++i) {
            switch (bufs[i].op) {
                case UNUSED:
                    if (off)
                    break;
                case READ_PNEDING:
                    break;
                case WRITE_PENDING:
                    break;
            }
        }
        if (numop == 0) {
            if (off >= sbuf.st_size) {
                break;
            }
        } else {
            if (aio_list(aio_list, NBUF, NULL) < 0) {
                err_sys("error");
            }
        }
    }

    bufs[0].aiocb.aio_fildes = output_fd;
    if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0) {
        err_sys("aio_fsync failed");
    }
    exit(0);
}