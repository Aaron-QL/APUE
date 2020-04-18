#include "../apue.3e/include/apue.h"
#include <pthread.h>

void cleanup(void *arg) {
    printf("cleanup: %s\n", (char *) arg);
}

void *thr_fn1(void *arg) {
    printf("thread 1 start\n");

    pthread_cleanup_push(cleanup, "thread1 first handler");

    printf("thread 1 push complete\n");

    return (void*) 1;
}

int main(void) {
    int err;
    pthread_t tid1;
    void *tret;

    if (pthread_create(&tid1, NULL, thr_fn1, (void *) 1) != 0) {
        err_sys("error");
    }

    if (pthread_join(tid1, &tret) != 0) {
        err_sys("error");
    }

    exit(0);
}
