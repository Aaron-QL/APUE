#include "../apue.3e/include/apue.h"
#include <pthread.h>

//线程安全版本的getenv

extern char **environ;

static pthread_once_t init_done = PTHREAD_ONCE_INIT;

static pthread_mutex_t env_mutex;

void thread_init(void) {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&env_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

int getenv_safe(const char* name, char *buf, size_t buffsize) {

    pthread_once(&init_done, thread_init);

    int len, old_len;

    len = strlen(name);

    pthread_mutex_lock(&env_mutex);

    for (int i = 0; environ[i] != NULL; ++i) {
        if (strncmp(name, environ[i], len) == 0 && environ[i][len] == '=') {
            //注意strlen的参数是指针，记得用&取地址
            old_len = strlen(&environ[i][len+1]);
            if (old_len >= buffsize) {
                pthread_mutex_unlock(&env_mutex);
                return ENOSPC;
            }
            strcpy(buf, &environ[i][len+1], old_len);
            //记得解锁
            pthread_mutex_unlock(&env_mutex);
            return 0;
        }
    }

    pthread_mutex_unlock(&env_mutex);
    return ENOENT;
}