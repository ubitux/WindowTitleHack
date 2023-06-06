# WindowTitleHack

This project is meant to prevent applications from changing their window title.
It is useful for tools looking at this property, such as [OBS Studio] (typically
with the builtin Automatic Scene Switcher) when used in combination with tools
that change their title bar all the time (Firefox, Krita, …).

See [this blog post][blog-post] for more details.

## Building

```sh
meson setup builddir
meson compile -C builddir
```

## Usage

```sh
LD_PRELOAD="builddir/libwth.so" WTH_TITLE="Krita4ever" krita
```

## Known limitations

- Only supports X11 and XCB (no Wayland support yet)
- Some applications may be strict with regards to `LD_PRELOAD`, typically
  Chromium, so it won't work with those
- All the windows are going to have the same title (splash screens, popups,
  menus, etc)
- Depending on the API used by the target application, UTF-8 and multibytes
  strings may not work as expected


[OBS Studio]: https://obsproject.com/
[blog-post]: https://blog.pkh.me/p/40-hacking-window-titles-to-help-obs.html
