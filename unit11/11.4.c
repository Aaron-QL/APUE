#include "../apue.3e/include/apue.h"
#include <pthread.h>

struct foo {
    int a, b, c, d;
};

void print_foo(const char *s, const struct foo *fp) {
    printf("%s", s);
    printf("  structure at 0x%lx\n", (unsigned long) fp);
    printf("  foo.a = %d\n", fp->a);
    printf("  foo.b = %d\n", fp->b);
    printf("  foo.c = %d\n", fp->c);
    printf("  foo.d = %d\n", fp->d);
}

void *thread_func1(void *arg) {
    struct foo foo = {1, 2, 3, 4};
    print_foo("thread 1: ", &foo);
    pthread_exit((void *) &foo);
}


int main(void) {
    pthread_t tid1;
    struct foo *foo_ptr;

    if (pthread_create(&tid1, NULL, thread_func1, NULL) != 0) {
        err_sys("error");
    }

    if (pthread_join(tid1, (void*)&foo_ptr) != 0) {
        err_sys("error");
    }

    print_foo("main: ", foo_ptr);

    exit(0);
}