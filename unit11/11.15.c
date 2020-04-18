#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

struct msg {
    int id;
    struct msg *next;
};

struct msg *msg_queue;

pthread_cond_t queue_ready = PTHREAD_COND_INITIALIZER;

pthread_mutex_t queue_block = PTHREAD_MUTEX_INITIALIZER;

void enqueue_msg(struct msg *msg_ptr) {
    printf("message enqueue: %d\n", msg_ptr->id);
    pthread_mutex_lock(&queue_block);

    msg_ptr->next = msg_queue;
    msg_queue = msg_ptr;

    pthread_mutex_unlock(&queue_block);

    pthread_cond_signal(&queue_ready);
}

void process_msg(char *name)
{
    struct msg *msg_ptr;

    while (1) {
        pthread_mutex_lock(&queue_block);

        while (msg_queue == NULL) {
            pthread_cond_wait(&queue_ready, &queue_block);
        }
        msg_ptr = msg_queue;
        msg_queue = msg_queue->next;

        pthread_mutex_unlock(&queue_block);

        //consume message
        printf("%s consume message: %d\n", name, msg_ptr->id);
    }
}

struct msg *msg_init(int id)
{
    struct msg *msg_ptr = malloc(sizeof(struct msg));
    if (msg_ptr != NULL) {
        msg_ptr->id = id;
    }
    return msg_ptr;
}

void *thread_func1(void *arg)
{
    process_msg("thread1");
    return (void *)0;
}

void *thread_func2(void *arg)
{
    process_msg("thread2");
    return (void *)0;
}


int main(void) {
    pthread_t tid1, tid2;

    if (pthread_create(&tid1, NULL, thread_func1, NULL) != 0) {
        printf("pthread_create error");
        exit(1);
    }

    if (pthread_create(&tid2, NULL, thread_func2, NULL) != 0) {
        printf("pthread_create error");
        exit(1);
    }

    for (int i = 0; i < 10; ++i) {
        struct msg *msg_ptr = msg_init(i);
        enqueue_msg(msg_ptr);
    }
    sleep(1);

    exit(0);
}