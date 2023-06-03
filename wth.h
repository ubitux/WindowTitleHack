#ifndef WTH_H
#define WTH_H

#include <pthread.h>

typedef void (*init_func_type)(void *user_arg);

const char *wth_get_title(void);
void wth_init_once(pthread_mutex_t *lock, int *initialized, init_func_type init_func, void *user_arg);

#endif
