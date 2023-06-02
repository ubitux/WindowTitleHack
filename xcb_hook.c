#include <dlfcn.h>
#include <string.h>
#include <xcb/xproto.h>

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

static xcb_change_property_func_type xcb_change_property_orig;

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
    /* Cache original function pointer */
    if (!xcb_change_property_orig)
        xcb_change_property_orig = dlsym(RTLD_NEXT, __func__);

    xcb_get_atom_name_reply_t *reply = xcb_get_atom_name_reply(conn, xcb_get_atom_name(conn, property), NULL);
    char *property_name = xcb_get_atom_name_name(reply);
    int len = xcb_get_atom_name_name_length(reply);
    const int chg = !strncmp(property_name, "WM_NAME", len) || !strncmp(property_name, "_NET_WM_NAME", len);

    if (chg) {
        const char *new_title = wth_get_title();
        return xcb_change_property_orig(conn, mode, window, property, type, format, (uint32_t)strlen(new_title), (const void *)new_title);
    }

    return xcb_change_property_orig(conn, mode, window, property, type, format, data_len, data);
}
