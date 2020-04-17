#include "../apue.3e/include/apue.h"
#include <pwd.h>

static void my_alarm(int signo)
{
    printf("alarm start\n");
    struct passwd *rootptr;

    if ((rootptr = getpwnam("root")) == NULL) {
        err_sys("getpwnam(root) error");
    }
    printf("pw_name = %s\n", rootptr->pw_name);
    alarm(1);
}

int main(void)
{
    struct passwd *ptr;

    signal(SIGALRM, my_alarm);
    alarm(1);

    for (;;) {
        if ((ptr = getpwnam("qinlong")) == NULL) {
            err_sys("getpwnam error");
        }
        if (strcmp(ptr->pw_name, "qinlong") != 0) {
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
        }
//        printf("pw_name = %s\n", ptr->pw_name);
    }
}