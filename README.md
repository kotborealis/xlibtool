# xlibtool

```
Usage: ./xlibtool [options] title

Options:
        --iconify       Iconify specified window
        --raise Raise specified window to top
        --lower Lower specified window to top
        --focus Bring focus to specified window
        --map   Map specified window
        --unmap Unmap specified window
		--is_hidden Is widnow hidden? (minified?)
		--toggle Raise window if hidden, iconify if not
```

References:

* https://specifications.freedesktop.org/wm-spec/1.3/ar01s05.html
* https://github.com/tmathmeyer/xprop/blob/master/xprop.c
* https://tronche.com/gui/x/xlib/ICC/client-to-window-manager/XIconifyWindow.html
* https://tronche.com/gui/x/xlib/window/XRaiseWindow.html
* https://tronche.com/gui/x/xlib/window/XLowerWindow.html
* https://tronche.com/gui/x/xlib/window/
* https://tronche.com/gui/x/xlib/window/stacking-order.html
* https://tronche.com/gui/x/xlib/input/XSetInputFocus.html