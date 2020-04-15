#include "../apue.3e/include/apue.h"
#include <setjmp.h>

//全局变量、静态变量和易失变量不受优化的影响

static void f1(int, int, int, int);
static void f2();

static jmp_buf jmp_buffer;

static int global_val;

int main(void)
{
    int auto_val;
    register int register_val;
    volatile int volatile_val;
    static int static_val;

    global_val = 1; auto_val = 2; register_val = 3; volatile_val = 4; static_val = 5;

    if (setjmp(jmp_buffer) != 0) {
        printf("after longjmp:\n");
        printf("global_val = %d, auto_val = %d, register_val = %d, volatile_val = %d, static_val = %d\n",
                global_val, auto_val, register_val, volatile_val, static_val);
        exit(0);
    }

    global_val = 11; auto_val = 22; register_val = 33; volatile_val = 44; static_val = 55;

    f1(auto_val, register_val, volatile_val, static_val);

    exit(0);
}

static void f1(int i, int j, int k, int l)
{
    printf("in f1:\n");
    printf("global_val = %d, auto_val = %d, register_val = %d, volatile_val = %d, static_val = %d\n",
           global_val, i, j, k, l);
    f2();
}

static void f2(void)
{
    longjmp(jmp_buffer, 2);
}