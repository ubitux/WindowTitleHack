#ifndef WTH_H
#define WTH_H

#include <pthread.h>

const char *wth_get_title(void);

/* The "once" API is similar to pthread_once but allows a custom function argument */
struct wth_once {
    pthread_mutex_t lock;
    int initialized;
};

#define WTH_ONCE_INITIALIZER {.lock=PTHREAD_MUTEX_INITIALIZER}

typedef void (*init_func_type)(void *user_arg);
void wth_init_once(struct wth_once *once, init_func_type init_func, void *user_arg);

#endif
