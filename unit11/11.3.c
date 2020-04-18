#include "../apue.3e/include/apue.h"
#include <pthread.h>

void *thread_func1(void *arg)
{
    printf("thread1 returning\n");
    return (void *)1;
}

void *thread_func2(void *arg)
{
    printf("thread2 returning\n");
    pthread_exit((void *) 2);
}

int main(void)
{
    pthread_t tid1, tid2;
    void *thread_return;

    if (pthread_create(&tid1, NULL, thread_func1, NULL) != 0) {
        err_sys("pthread_create error");
    }


    if (pthread_create(&tid2, NULL, thread_func2, NULL) != 0) {
        err_sys("pthread_create error");
    }


    if (pthread_join(tid1, &thread_return) != 0) {
        err_sys("pthread_join error");
    }

    printf("thread 1 exit code %ld\n", (long)thread_return);

    if (pthread_join(tid2, &thread_return) != 0) {
        err_sys("pthread_join error");
    }

    printf("thread 2 exit code %ld\n", (long)thread_return);

    exit(0);
}