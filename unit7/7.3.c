#include "../apue.3e/include/apue.h"
#include <stdio.h>

//终止处理程序 + 输出字符串的4种写法
static void exit1(void);
static void exit2(void);
static void exit3(void);
static void exit4(void);

int main(void)
{
    if (atexit(exit1) != 0) {
        err_sys("atexit error");
    }

    if (atexit(exit2) != 0) {
        err_sys("atexit error");
    }

    if (atexit(exit3) != 0) {
        err_sys("atexit error");
    }

    if (atexit(exit4) != 0) {
        err_sys("atexit error");
    }

    printf("main is done\n");
    exit(0);
}

void exit1(void)
{
    printf("first exit hanler\n");
}

void exit2(void)
{
    write(STDOUT_FILENO, "second exit handler\n", 20);
}

void exit3(void)
{
    fputs("third exit handler\n", stdout);
}

void exit4(void)
{
    fwrite("fourth exit handler\n", 20, 1, stdout);
}