#include <dlfcn.h>
#include <string.h>
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

static struct wth_once once = WTH_ONCE_INITIALIZER;
static const char *new_title;
static XChangeProperty_func_type XChangeProperty_orig;
static Atom _NET_WM_NAME;

static void init_once(void *user_arg)
{
    /* Cache original function pointer */
    XChangeProperty_orig = dlsym(RTLD_NEXT, "XChangeProperty");

    new_title = wth_get_title();

    Display *display = user_arg;
    _NET_WM_NAME = XInternAtom(display, "_NET_WM_NAME", 0);
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
    wth_init_once(&once, init_once, display);

    if (property == XA_WM_NAME || property == _NET_WM_NAME) {
        data = (const unsigned char *)new_title;
        nelements = (int)strlen(new_title);
    }

    return XChangeProperty_orig(display, w, property, type, format, mode, data, nelements);
}
