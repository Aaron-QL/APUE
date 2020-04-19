#include "../apue.3e/include/apue.h"
#include <pthread.h>

#define MAXSTRINGSZ 1024;
pthread_key_t key;
static pthread_once_t init_done = PTHREAD_ONCE_INIT;
static pthread_mutex_t env_mutex = PTHREAD_MUTEX_INITIALIZER;

static void thread_init(void) {
    pthread_key_create(&key, free);
}

extern char **environ;

char *getenv(char *name) {

    int len = sizeof(name);
    char *env_buf;

    pthread_once(&init_done, thread_init);
    pthread_mutex_lock(&env_mutex);

    //获取私有数据，不存在的话则主动分配
    if ((env_buf = (char *)pthread_getspecific(key)) == NULL) {
        if ((env_buf = malloc(MAXSTRINGSZ)) == NULL) {
            pthread_mutex_unlock(&env_mutex);
            return NULL;
        }
        pthread_setspecific(&key, env_buf);
    }

    for (int i = 0; environ[i] != NULL; ++i) {
        if (strncmp(name, environ[i], len) == 0 && environ[i][len+1] == '=') {
            //strncpy前两个参数都是指针，记得加&取址！！！
            strncpy(env_buf, &environ[i][len+1], MAXSTRINGSZ - 1);
            pthread_mutex_unlock(&env_mutex);
            return env_buf;
        }
    }

    pthread_mutex_unlock(&env_buf);
    return NULL;
}