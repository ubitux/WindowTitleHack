#include <stdlib.h>
#include <pthread.h>

#include "wth.h"

const char *wth_get_title(void)
{
    const char *env_title = getenv("WTH_TITLE");
    return env_title ? env_title : "Use WTH_TITLE environment variable to replace this";
}

void wth_init_once(struct wth_once *once, init_func_type init_func, void *user_arg)
{
    pthread_mutex_lock(&once->lock);
    if (!once->initialized) {
        init_func(user_arg);
        once->initialized = 1;
    }
    pthread_mutex_unlock(&once->lock);
}
