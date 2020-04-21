#include "../apue.3e/include/apue.h"

static int pfd1[2], pfd2[2];

void TELL_WAIT()
{
    if (pipe(pfd1) < 0 || pipe(pfd2) < 0) {
        err_sys("error");
    }
}

void TELL_CHILD()
{
    if (write(pfd1[1], "p", 1) != 1) {
        err_sys("error");
    }
}

void TELL_PARENT()
{
    if (write(pfd2[1], "c", 1) != 1) {
        err_sys("error");
    }
}

void WAIT_PATENT()
{
    char c;
    if (read(pfd2[0], &c, 1) != 1) {
        err_sys("error");
    }

    if (c != 'p') {
        err_sys("error");
    }
}

void WAIT_PARENT()
{
    char c;
    if (read(pfd1[0], &c, 1) != 1) {
        err_sys("error");
    }
    if (c != 'c') {
        err_sys("error");
    }
}