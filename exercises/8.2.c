#include "../apue.3e/include/apue.h"

static int f1(void);
static void f2(int);

int main(void)
{
    f2(f1());
    exit(0);
}

static int f1(void)
{
    int pid;
    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    }

    return pid;
}

static void f2(int p)
{
    char buf[10];
    int i;
    for (i = 0; i < 10; i++) {
        fputc(buf[i], stdout);
        buf[i] = p;
    }
}