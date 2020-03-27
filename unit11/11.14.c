#include "../apue.3e/include/apue.h"
#include <stdlib.h>
#include <pthread.h>

struct job {
    struct job *next;
    struct job *prev;
    pthread_t id;
};

struct queue {
    struct job *head;
    struct job *tail;
    pthread_rwlock_t lock;
};

int queue_init(struct queue *qp)
{
    int err;
    qp->head = NULL;
    qp->tail = NULL;
    if ((err = pthread_rwlock_init(&qp->lock, NULL)) != 0) {
        return err;
    }

    return 0;
}

void job_insert(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->lock);
    jp->next = qp->head;
    jp->prev = NULL;
    if (qp->head == NULL) {
        qp->tail = jp;
    } else {
        qp->head->prev = jp;
    }
    qp->head = jp;
    pthread_rwlock_unlock(&qp->lock);
}

void job_append(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->lock);
    jp->next = NULL;
    jp->prev = qp->tail;
    if (qp->head == NULL) {
        qp->head = jp;
    } else {
        qp->tail->next = jp;
    }
    qp->tail = jp;
    pthread_rwlock_unlock(&qp->lock);
}

void job_remove(struct queue *qp, struct job *jp)
{
    pthread_rwlock_wrlock(&qp->lock);
    if (jp == qp->head) {
        qp->head = jp->next;
        if (qp->tail == jp) {
            qp->tail = NULL;
        } else {
            jp->next->prev = jp->prev;
        }
    } else if (jp == qp->tail) {
        qp->tail = jp->prev;
        jp->prev->next = jp->next;
    } else {
        jp->prev->next = jp->next;
        jp->next->prev = jp->prev;
    }
    pthread_rwlock_unlock(&qp->lock);
}

struct job *job_find(struct queue *qp, pthread_t id)
{
    struct job *jp;

    if (pthread_rwlock_rdlock(&qp->q_lock) != 0)
        return(NULL);

    for (jp = qp->q_head; jp != NULL; jp = jp->j_next)
        if (pthread_equal(jp->j_id, id))
            break;

    pthread_rwlock_unlock(&qp->q_lock);
    return(jp);
}
