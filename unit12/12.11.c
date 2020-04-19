#include "../apue.3e/include/apue.h"
#include <string.h>

#define BUFFSIZE 1024
static char env_buf[BUFFSIZE];

extern char **environ;


char *get_env(const char *name)
{
    int len = strlen(name);
    for (int i = 0; environ[i] != NULL; ++i) {
        if (strncmp(name, environ[i], len) == 0 && environ[i][len] == '=') {
            strncpy(env_buf, &environ[i][len+1], BUFFSIZE - 1);
            return env_buf;
        }
    }
    return NULL;
}
