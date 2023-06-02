#include <stdlib.h>

#include "wth.h"

static const char *cached_title;

const char *wth_get_title(void)
{
    if (cached_title)
        return cached_title;
    const char *env_title = getenv("WTH_TITLE");
    cached_title = env_title ? env_title : "Use WTH_TITLE environment variable to replace this";
    return cached_title;
}