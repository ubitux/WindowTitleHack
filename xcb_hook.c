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
static xcb_change_property_func_type xcb_change_property_orig;
static xcb_atom_t _NET_WM_NAME;

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

    /* Cache original function pointer */
    if (!xcb_change_property_orig)
        xcb_change_property_orig = dlsym(RTLD_NEXT, __func__);

    if (!_NET_WM_NAME) {
        const xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, 0, strlen("_NET_WM_NAME"), "_NET_WM_NAME");
        xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
        if (reply)
            _NET_WM_NAME = reply->atom;
        free(reply);
    }

    pthread_mutex_unlock(&lock);

    if (property == XCB_ATOM_WM_NAME || property == _NET_WM_NAME) {
        const char *new_title = wth_get_title();
        return xcb_change_property_orig(conn, mode, window, property, type, format, (uint32_t)strlen(new_title), (const void *)new_title);
    }

    return xcb_change_property_orig(conn, mode, window, property, type, format, data_len, data);
}
