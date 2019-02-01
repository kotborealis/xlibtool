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

/**
 * Проверка, совпадает ли имя окна с needle
**/
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

/**
 * Поиск окна по имени needle
**/
Window window_from_name_search(Display *display, Window current, char const *needle) {
  Window retval, root, parent, *children;
  unsigned children_count;
  char *name = NULL;

  // если текущее окно подходит, выкидываем его
  if(match_window_name(display, current, needle))
    return current;

  retval = 0;

  // если нет, рекурсивно перебираем его детей
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

/**
  * Поиск окна по имени, обёртка
**/
Window window_from_name(Display *display, char const *name) {
  return window_from_name_search(display, XDefaultRootWindow(display), name);
}

void usage(char* cmd){
  printf("Usage: %s [options] title\n", cmd);
  printf("\n");
  printf("Options:\n");
  printf("\t--iconify\tIconify specified window\n");
  printf("\t--raise\tRaise specified window to top\n");
  printf("\t--lower\tLower specified window to top\n");
  printf("\t--focus\tBring focus to specified window\n");
  printf("\t--map\tMap specified window\n");
  printf("\t--unmap\tUnmap specified window\n");
}

int main(int argc, char** argv){
  int retval = 0;

  int lower = 0;
  int iconify = 0;
  int raise = 0;
  int focus = 0;
  int map = 0;
  int unmap = 0;

  char* title = NULL;

  if(argc < 2){
    usage(argv[0]);
    return 1;
  }
  else{
    for(int i = 1; i < argc; i++){
      if(argv[i][0] == '-' && argv[i][1] == '-'){
        char *_ = argv[i] + 2;
        iconify += strcmp(_, "iconify") == 0;
        raise += strcmp(_, "raise") == 0;
        lower += strcmp(_, "lower") == 0;
        focus += strcmp(_, "focus") == 0;
        map += strcmp(_, "map") == 0;
        unmap += strcmp(_, "unmap") == 0;
      }
      else{
        title = argv[i];
      }
    }
  }

  if(!title){
    usage(argv[0]);
    return 1;
  }

  Display *display = XOpenDisplay(NULL);

  Window window = window_from_name(display, title);

  if(window == 0){
    printf("Window with title `%s` not found, halting\n", title);
    retval = 1;
  }
  else{
    printf("Window with title `%s` found with id # 0x%lx\n", title, window);

    if(lower)
      XLowerWindow(display, window);

    if(iconify)
      XIconifyWindow(display, window, 0);

    if(raise)
      XRaiseWindow(display, window);

    if(focus)
      XSetInputFocus(display, window, RevertToNone, CurrentTime);

    if(map)
      XMapWindow(display, window);

    if(unmap)
      XUnmapWindow(display, window);
  }

  XCloseDisplay(display);

  return retval;
}