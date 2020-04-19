#include "../apue.3e/include/apue.h"
#include <pthread.h>

static int quit_flag = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait_condition = PTHREAD_COND_INITIALIZER;

static sigset_t new_mask;

void *thread_listen(void *arg) {
    int signo;
    while (1) {
        if (sigwait(&new_mask, &signo) != 0) {
            err_sys("error");
        }
        switch (signo) {
            case SIGINT:
                printf("\ninterrupt\n");
                break;
            case SIGQUIT:
                pthread_mutex_lock(&lock);
                quit_flag = 1;
                pthread_mutex_unlock(&lock);
                pthread_cond_signal(&wait_condition);
                return (void *) 0;
            default:
                printf("unexpected signal %d\n", signo);
                exit(1);
        }
    }
}

int main(void) {
    pthread_t tid;
    sigset_t old_mask;

    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGINT);
    sigaddset(&new_mask, SIGQUIT);

    if (pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask) != 0) {
        err_sys("error");
    }

    if (pthread_create(&tid, NULL, thread_listen, NULL) != 0) {
        err_sys("error");
    }


    pthread_mutex_lock(&lock);
    while (quit_flag == 0) {
        pthread_cond_wait(&wait_condition, &lock);
    }
    pthread_mutex_unlock(&lock);

    quit_flag = 0;
    if (pthread_sigmask(SIG_SETMASK, &old_mask, NULL) != 0) {
        err_sys("error");
    }

    exit(0);
}