#include "../apue.3e/include/apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long) id) % NNHASH)

struct foo {
    int id;
    int count;
    pthread_mutex_t lock;
    struct foo *next;
};

struct foo *foo_buffer[NHASH];

pthread_mutex_t hash_lock = PTHREAD_MUTEX_INITIALIZER;

struct foo *foo_alloc(int id) {
    int hash_id = HASH(id);

    struct foo *foo_ptr = malloc(sizeof(struct foo));
    if (foo_ptr == NULL) {
        return NULL;
    }

    foo_ptr->id = id;
    foo_ptr->count = 0;
    pthread_mutex_init(&foo_ptr->lock);

    pthread_mutex_lock(&ash_lock);
    foo_ptr->next = foo_buffer[hash_id];
    foo_buffer[hash_id] = foo_ptr;
    pthread_mutex_unlock(&hash_lock);

    return foo_ptr;
}

void foo_hold(struct foo *foo_ptr)
{
    pthread_mutex_lock(&foo_ptr->lock);
    foo_ptr->count++;
    pthread_mutex_unlock(&foo_ptr->lock);
}

struct foo *foo_find(int id)
{
    struct foo *foo_ptr;
    int hash_id = HASH(id);
    pthread_mutex_lock(&hash_lock);
    for (foo_ptr = foo_buffer[hash_id]; foo_ptr != NULL; foo_ptr = foo_ptr->next) {
        if (foo_ptr->id == id) {
            foo_hold(foo_ptr);
            break;
        }
    }
    pthread_mutex_unlock(&hash_lock);

    return foo_ptr;
}

void foo_release(struct foo *foo_ptr)
{
    pthread_mutex_lock(&foo_ptr->lock);

    if (foo_ptr->count == 1) {
        pthread_mutex_unlock(&foo_ptr->lock);
        pthread_mutex_lock(&hash_lock);
        pthread_mutex_lock(&foo_ptr->lock);
        if (foo_ptr->count == 1) {
            int hash_id = HASH(foo_ptr->id);
            struct foo *foo_temp = foo_buffer[hash_id];
            if (foo_temp == foo_ptr) {
                foo_buffer[hash_id] = foo_ptr->next;
            } else {
                while (foo_temp->next != NULL) {
                    if (foo_temp->next == foo_ptr) {
                        foo_temp->next = foo_ptr->next;
                        break;
                    }
                    foo_temp = foo_temp->next;
                }
            }
            pthread_mutex_unlock(&hash_lock);
            pthread_mutex_unlock(&foo_ptr->lock);
            pthread_mutex_destroy(&foo_ptr->lock);
            free(foo_ptr);
        } else {
            foo_ptr->count--;
            pthread_mutex_unlock(&foo_ptr->lock);
            pthread_mutex_unlock(&hash_lock);
        }
    } else {
        foo_ptr->count--;
        pthread_mutex_unlock(&foo_ptr->lock);
    }

}