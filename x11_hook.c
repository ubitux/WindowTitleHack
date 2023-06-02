#include <dlfcn.h>
#include <string.h>
#include <X11/Xlib.h>

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

static XChangeProperty_func_type XChangeProperty_orig;

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
    /* Cache original function pointer */
    if (!XChangeProperty_orig)
        XChangeProperty_orig = dlsym(RTLD_NEXT, __func__);

    char *property_name = XGetAtomName(display, property);
    const int chg = !strcmp(property_name, "WM_NAME") || !strcmp(property_name, "_NET_WM_NAME");
    XFree(property_name);

    if (chg) {
        const char *new_title = wth_get_title();
        return XChangeProperty_orig(display, w, property, type, format, mode, (const unsigned char *)new_title, (int)strlen(new_title));
    }

    return XChangeProperty_orig(display, w, property, type, format, mode, data, nelements);
}
