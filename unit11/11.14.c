#include "../apue.3e/include/apue.h"
#include <stdlib.h>
#include <pthread.h>

struct job {
    pthread_t id;
    struct job *prevent;
    struct job *next;
};

struct queue {
    struct job *head;
    struct job *tail;
    pthread_rwlock_t lock;
};

int queue_init(struct queue *queue_ptr) {
    queue_ptr->head = NULL;
    queue_ptr->tail = NULL;
    return pthread_rwlock_init(&queue_ptr->lock, NULL);
}

void job_insert(struct queue *queue_ptr, struct job *job_ptr) {
    pthread_rwlock_wrlock(&queue_ptr->lock);

    job_ptr->next = queue_ptr->head;
    job_ptr->prevent = NULL;
    if (queue_ptr->head == NULL) {
        queue_ptr->tail = job_ptr;
    } else {
        queue_ptr->head->prevent = job_ptr;
    }
    queue_ptr->head = job_ptr;

    pthread_rwlock_unlock(&queue_ptr->lock);
}

void job_append(struct queue *queue_ptr, struct job * job_ptr) {
    pthread_rwlock_wrlock(&queue_ptr->lock);

    job_ptr->prevent = queue_ptr->tail;
    job_ptr->next = NULL;
    if (queue_ptr->tail == NULL) {
        queue_ptr->head = job_ptr;
    } else {
        queue_ptr->tail->next = job_ptr;
    }
    queue_ptr->tail = job_ptr;

    pthread_rwlock_unlock(&queue_ptr->lock);
}

void job_remove(struct queue *queue_ptr, struct job *job_ptr) {
    pthread_rwlock_wrlock(&queue_ptr->lock);

    if (job_ptr == queue_ptr->head) {
        queue_ptr->head = job_ptr->next;
        if (queue_ptr->tail == job_ptr) {
            queue_ptr->tail = NULL;
        } else {
            queue_ptr->head->prevent = job_ptr->prevent;
        }
    } else if (job_ptr == queue_ptr->tail) {
        queue_ptr->tail = job_ptr->prevent;
        job_ptr->prevent->next = job_ptr->next;
    } else {
        job_ptr->prevent->next = job_ptr->next;
        job_ptr->next->prevent = job_ptr->prevent;
    }

    pthread_rwlock_unlock(&queue_ptr->lock);
}

struct job *job_find(struct queue *queue_ptr, pthread_t tid) {
    pthread_rwlock_rdlock(&queue_ptr->lock);
    struct job *job_ptr;
    for (job_ptr = queue_ptr->head; job_ptr != NULL; job_ptr = job_ptr->next) {
        //比较线程ID时使用pthread_equal函数
        if (pthread_equal(job_ptr->id, tid)) {
            break;
        }
    }
    pthread_rwlock_unlock(&queue_ptr->lock);
    return job_ptr;
}