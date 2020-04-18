#include "../apue.3e/include/apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct foo {
    int id;
    int count;
    pthread_mutex_t lock;
};

struct foo *foo_alloc(int id)
{
    struct foo *foo_ptr = malloc(sizeof(struct foo));
    if (foo_ptr == NULL) {
        return foo_ptr;
    }

    foo_ptr->id = id;
    foo_ptr->count = 0;
    if (pthread_mutex_init(&foo_ptr->lock, NULL) != 0) {
        free(foo_ptr);
        return NULL;
    }

    return foo_ptr;
}

void foo_hold(struct foo foo_ptr)
{
    pthread_mutex_lock(&foo_ptr.lock);
    foo_ptr.count++;
    pthread_mutex_unlock(&foo_ptr.lock);
}

void foo_release(struct foo foo_ptr)
{
    pthread_mutex_lock(&foo_ptr.lock);
    if (--foo_ptr.count == 0) {
        pthread_mutex_unlock(&foo_ptr.lock);
        pthread_mutex_destroy(&foo_ptr.lock);
        free(foo_ptr);
    } else {
        pthread_mutex_unlock(&foo_ptr.lock);
    }
}