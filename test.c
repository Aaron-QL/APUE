#include "apue.3e/include/apue.h"

int main(void)
{
    clock_t t0 = clock();

    clock_t t1 = clock();
    printf("%Lf", (long double)(t1 - t0));
}