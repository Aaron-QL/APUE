#include "../apue.3e/include/apue.h"

int main()
{
    if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1) {
        printf("cannot seek\n");
    } else {
        puts("seek OK\n");
    }
}