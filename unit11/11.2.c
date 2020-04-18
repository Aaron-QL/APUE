#include "../apue.3e/include/apue.h"
#include <pthread.h>

pthread_t new_tid;

void print_ids(const char *s)
{
    pid_t pid = getppid();
    pthread_t tid = pthread_self();

    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long) pid, (unsigned long) tid, (unsigned long) tid);
}

void *thread_function(void *arg)
{
    print_ids("new thread : ");
    return (void *) 0;
}

int main(void)
{
    int err;
    err = pthread_create(&new_tid, NULL, thread_function, NULL);

    if (err != 0) {
        err_exit(err, "can't create thread");
    }

    print_ids("main thread: ");
    sleep(1);
    exit(0);
}