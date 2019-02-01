# xlibtool

Ищет окно на рабочем столе и выполняет с ним операции.

* `iconify` --- минимизировать окно
* `raise` --- поднять окно наверх

* Usage: `xlibtool [iconify|raise] <title>`
* Dependencies: `xlib`
* Build: `make`

References:

* https://github.com/tmathmeyer/xprop/blob/master/xprop.c
* https://tronche.com/gui/x/xlib/ICC/client-to-window-manager/XIconifyWindow.html
* https://tronche.com/gui/x/xlib/window/XRaiseWindow.html
* https://tronche.com/gui/x/xlib/window/
* https://tronche.com/gui/x/xlib/window/stacking-order.html