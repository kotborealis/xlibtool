#pragma once

#include <X11/Xlib.h>

int is_window_hidden(Display *display, Window window);
int match_window_name(Display *display, Window window, const char *needle);
Window window_from_name_search(Display *display, Window current, char const *needle);
Window window_from_name(Display *display, char const *name);
int toggle_window(Display *display, Window window);