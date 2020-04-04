#include "../apue.3e/include/apue.h"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>


int makethread(void *(*fn)(void *), void *arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;

    if ((err = pthread_attr_init(&attr)) != 0) {
        return err;
    }

    if ((err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)) != 0) {
        pthread_attr_destroy(&attr);
        return err;
    }

    err = pthread_create(&tid, &attr, fn, arg);
    pthread_attr_destroy(&attr);
    return err;
}

//任务结构
struct todo_info {
    void (*to_fn)(void *);
    void *to_arg;
    struct timespec to_wait;
};

#define SECTONSEC  1000000000	/* seconds to nanoseconds */

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM)	nanosleep((REQ), (REM))
#endif


#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000		/* microseconds to nanoseconds */

void
clock_gettime(int id, struct timespec *tsp)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	tsp->tv_sec = tv.tv_sec;
	tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

//timeout_helper
void *timeout_helper(void *arg)
{
    struct todo_info *todo_ptr = (struct todo_info*) arg;
    nanosleep(&todo_ptr->to_wait, NULL);
    (*todo_ptr->to_fn)(todo_ptr->to_arg);
    free(arg);
    return 0;
}

//timeout
void timeout(struct timespec* when, void (*func)(void *), void *arg)
{
    struct timespec now;
    struct todo_info *todo_ptr;
    int err;
    clock_gettime(CLOCK_REALTIME, &now);
    if (when->tv_sec > now.tv_sec || (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
        todo_ptr = malloc(sizeof(struct todo_info));
        if (todo_ptr != NULL) {
            todo_ptr->to_fn = func;
            todo_ptr->to_arg = arg;
            todo_ptr->to_wait.tv_sec = when->tv_sec - now.tv_sec;
            if (when->tv_nsec >= now.tv_nsec) {
                todo_ptr->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
            } else {
                todo_ptr->to_wait.tv_sec--;
                todo_ptr->to_wait.tv_nsec = when->tv_nsec + SECTONSEC - now.tv_nsec;
            }
            err = makethread(timeout_helper, todo_ptr);
            if (err == 0) {
                return;
            } else {
                free(todo_ptr);
            }
        }
    }
    (*func)(arg);
}

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

//retry
void retry(void *arg)
{
    pthread_mutex_lock(&mutex);
    /*
     *
     */
    pthread_mutex_unlock(&mutex);
}

//main
int main(void)
{
    int err;
    int condition = 1;
    int arg;
    struct timespec when;

    if ((err = pthread_mutexattr_init(&attr)) != 0) {
        err_exit(err, "pthread_mutexattr_init failed");
    }

    if ((err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)) != 0) {
        err_exit(err, "can't set recursive type");
    }

    if ((err = pthread_mutex_init(&mutex, &attr)) != 0) {
        err_exit(err, "can't create recursive mutex");
    }

    pthread_mutex_lock(&mutex);

    if (condition) {
        clock_gettime(CLOCK_REALTIME, &when);
        when.tv_sec += 10;
        timeout(&when, retry, (void *)((unsigned long) arg));
    }

    pthread_mutex_unlock(&mutex);

    exit(0);
}