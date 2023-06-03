#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <xcb/xcb.h>
#include <xcb/xcb_atom.h>

#include "wth.h"

typedef xcb_void_cookie_t (*xcb_change_property_func_type)(
    xcb_connection_t *conn,
    uint8_t mode,
    xcb_window_t window,
    xcb_atom_t property,
    xcb_atom_t type,
    uint8_t format,
    uint32_t data_len,
    const void *data
);

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int initialized;
static const char *new_title;
static xcb_change_property_func_type xcb_change_property_orig;
static xcb_atom_t _NET_WM_NAME;

static void init_once(xcb_connection_t *conn)
{
    /* Cache original function pointer */
    xcb_change_property_orig = dlsym(RTLD_NEXT, "xcb_change_property");

    const xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, strlen("_NET_WM_NAME"), "_NET_WM_NAME");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
    if (reply)
        _NET_WM_NAME = reply->atom;
    free(reply);

    new_title = wth_get_title();
}

xcb_void_cookie_t xcb_change_property(
    xcb_connection_t *conn,
    uint8_t mode,
    xcb_window_t window,
    xcb_atom_t property,
    xcb_atom_t type,
    uint8_t format,
    uint32_t data_len,
    const void *data
) {
    pthread_mutex_lock(&lock);
    if (!initialized) {
        init_once(conn);
        initialized = 1;
    }
    pthread_mutex_unlock(&lock);

    if (property == XCB_ATOM_WM_NAME || property == _NET_WM_NAME) {
        data = (const void *)new_title;
        data_len = (uint32_t)strlen(new_title);
    }

    return xcb_change_property_orig(conn, mode, window, property, type, format, data_len, data);
}
