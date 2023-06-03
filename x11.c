#include <dlfcn.h>
#include <string.h>
#include <pthread.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "wth.h"

typedef int (*XChangeProperty_func_type)(
    Display *display,
    Window w,
    Atom property,
    Atom type,
    int format,
    int mode,
    const unsigned char *data,
    int nelements
);

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int initialized;
static const char *new_title;
static XChangeProperty_func_type XChangeProperty_orig;
static Atom _NET_WM_NAME;

static void init_once(Display *display)
{
    /* Cache original function pointer */
    XChangeProperty_orig = dlsym(RTLD_NEXT, "XChangeProperty");

    _NET_WM_NAME = XInternAtom(display, "_NET_WM_NAME", 0);

    new_title = wth_get_title();
}

int XChangeProperty(
    Display *display,
    Window w,
    Atom property,
    Atom type,
    int format,
    int mode,
    const unsigned char *data,
    int nelements
) {
    pthread_mutex_lock(&lock);
    if (!initialized) {
        init_once(display);
        initialized = 1;
    }
    pthread_mutex_unlock(&lock);

    if (property == XA_WM_NAME || property == _NET_WM_NAME) {
        data = (const unsigned char *)new_title;
        nelements = (int)strlen(new_title);
    }

    return XChangeProperty_orig(display, w, property, type, format, mode, data, nelements);
}
