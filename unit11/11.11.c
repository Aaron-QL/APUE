#include "../apue.3e/include/apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NHASH 29
#define HASH(id) (((unsigned long) id) % NHASH)

struct foo {
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
    struct foo *f_next;
};

struct foo *fh[NHASH];

pthread_mutex_t hashlock = PTHREAD_MUTEX_INITIALIZER;

struct foo* foo_alloc(int id)
{
    struct foo *fp;
    int hashid;
    if ((fp = malloc(sizeof(struct foo))) == NULL) {
        err_sys("malloc failed");
    }
    fp->f_id = id;
    fp->f_count = 1;
    if (pthread_mutex_init(&fp->f_lock, NULL) != 0) {
        free(fp);
        err_sys("pthread_mutex_init failed");
    }
    hashid = HASH(id);
    //先锁住哈希表
    pthread_mutex_lock(&hashlock);
    //插入哈希表：
    fp->f_next = fh[hashid];
    fh[hashid] = fp;
    pthread_mutex_lock(&fp->f_lock);
    //解锁哈希表
    pthread_mutex_unlock(&hashlock);
    /* ... 其他初始化工作 ... */
    pthread_mutex_unlock(&fp->f_lock);

    return fp;
}

void foo_hold(struct foo *fp) /* add a reference to the object */
{
    pthread_mutex_lock(&fp->f_lock);
    fp->f_count++;
    pthread_mutex_unlock(&fp->f_lock);
}

struct foo* foo_find(int id)
{
    struct foo* fp;
    pthread_mutex_lock(&hashlock);
    for (fp = fh[HASH(id)]; fp != NULL; fp = fp->f_next) {
        if (fp->f_id == id) {
            foo_hold(fp);
            break;
        }
    }
    pthread_mutex_unlock(&hashlock);
    return fp;
}

void foo_rele(struct foo *fp)
{
    pthread_mutex_lock(&fp->f_lock);
    if (fp->f_count == 1) {
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_lock(&hashlock);
        pthread_mutex_lock(&fp->f_lock);
        if (fp->f_count != 1) {
            fp->f_count--;
            pthread_mutex_unlock(&fp->f_lock);
            pthread_mutex_unlock(&hashlock);
            return;
        }
        int hashid = HASH(fp->f_id);
        struct foo* tfp;
        tfp = fh[hashid];
        if (tfp == fp) {
            fh[hashid] = fp->f_next;
        } else {
            while (tfp->f_next != fp) {
                tfp = tfp->f_next;
            }
            tfp->f_next = fp->f_next;
        }
        pthread_mutex_unlock(&hashlock);
        pthread_mutex_unlock(&fp->f_lock);
        pthread_mutex_destroy(&fp->f_lock);
        free(fp);
    } else {
        fp->f_count--;
        pthread_mutex_unlock(&fp->f_lock);
    }
}

void *thr_fn(void *arg) {
    foo_hold(arg);
    foo_rele(arg);
    return ((void *) 0);
}

int main()
{
    pthread_t tid;
    struct foo *fp = foo_alloc(1);

    if (pthread_create(&tid, NULL, thr_fn, fp) != 0) {
        exit(3);
    }
    sleep(1);
    printf("%d: %d\n", fp->f_id, fp->f_count);
}