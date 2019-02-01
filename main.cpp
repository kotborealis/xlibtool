#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

int match_window_name(Display *display, Window window, const char *needle) {
  Atom netWmName;
  Atom utf8;
  Atom actual_type;
  int actual_format;
  unsigned long nitems, bytes_after, nbytes;
  char** data;
  int status;

  netWmName = XInternAtom(display, "_NET_WM_NAME", False);
  utf8 = XInternAtom(display, "UTF8_STRING", False);

  status = XGetWindowProperty(display, window, netWmName, 
    0, 0x77777777, 
    False, utf8, 
    &actual_type, &actual_format, &nitems, &bytes_after, (unsigned char **) &data);

  if (status == BadWindow)
    printf("window id # 0x%lx does not exists!", window);
  if (status != Success)
    printf("XGetWindowProperty failed!");

  if (actual_format == 32)
    nbytes = sizeof(long);
  else if (actual_format == 16)
    nbytes = sizeof(short);
  else if (actual_format == 8)
    nbytes = 1;
  else if (actual_format == 0)
    nbytes = 0;

  char *name = (char *)malloc(nbytes * nitems + 1);
  sprintf(name, "%.*s", nbytes * nitems, data);
  XFree(data);

  int retval = strcmp(name, needle) == 0;

  free(name);

  return retval;
}

Window window_from_name_search(Display *display, Window current, char const *needle) {
  Window retval, root, parent, *children;
  unsigned children_count;
  char *name = NULL;

  if(match_window_name(display, current, needle))
    return current;

  retval = 0;

  if(0 != XQueryTree(display, current, &root, &parent, &children, &children_count)) {
    unsigned i;
    for(i = 0; i < children_count; ++i) {
      Window win = window_from_name_search(display, children[i], needle);

      if(win != 0) {
        retval = win;
        break;
      }
    }

    XFree(children);
  }

  return retval;
}

// frontend function: open display connection, start searching from the root window.
Window window_from_name(char const *name) {
  Display *display = XOpenDisplay(NULL);
  Window w = window_from_name_search(display, XDefaultRootWindow(display), name);
  XCloseDisplay(display);
  return w;
}

int main(int argc, char** argv){
  if(argc < 2) 
    return 0;

  Window window = window_from_name(argv[1]);
  if(window != 0)
    printf("window id # 0x%lx\n", window);
  else
    printf("nothing\n");
  return 0;
}