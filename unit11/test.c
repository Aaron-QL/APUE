#include "../apue.3e/include/apue.h"
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready = PTHREAD_COND_INITIALIZER;

void *fn(void *arg)
{
    printf("fn start\n");

    printf("wait: %d\n", pthread_cond_wait(&ready, &lock));

    printf("fn continue\n");


    printf("fn lock: %d\n", pthread_mutex_lock(&lock));
//    pthread_mutex_unlock(&lock);

    return ((void *) 0);
}

int main()
{

    printf("main start\n");
    pthread_t id = malloc(sizeof(pthread_t));
    printf("create: %d\n", pthread_create(&id, NULL, fn, NULL));

    printf("lock: %d\n", pthread_mutex_lock(&lock));

    sleep(2);

    printf("signal: %d\n", pthread_cond_signal(&ready));


}