#include "../apue.3e/include/apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

struct foo* foo_alloc(int id)
{
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) == NULL) {
        exit(1);
    }
    fp->f_count = 1;
    fp->f_id = id;
    if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
        free(fp);
        exit(2);
    }
    return fp;
}

void foo_hold(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

void foo_rele(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    if (--fp->f_count == 0) {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        pthread_mutex_unlock(&fp->f_lock);
    }
}

void *thr_fn(void *arg) {
    foo_hold(arg);
    return ((void *) 0);
}

int main()
{
    pthread_t tid;
    struct foo *fp = foo_alloc(1);
    if (pthread_create(&tid, NULL, thr_fn, fp) != 0) {
        exit(3);
    }
    printf("%d: %d\n", fp->f_id, fp->f_count);
}