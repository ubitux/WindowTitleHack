#include <stdlib.h>

#include "wth.h"

const char *wth_get_title(void)
{
    const char *env_title = getenv("WTH_TITLE");
    return env_title ? env_title : "Use WTH_TITLE environment variable to replace this";
}