#include "xlibtool.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

/**
 * Свёрну то ли окно? :thinking:
**/
int is_window_hidden(Display *display, Window window){
	Atom netWmState;
	Atom netWmStateHidden;
	Atom actual_type;
	int actual_format;
	unsigned long nitems, bytes_after, nbytes;
	Atom *atoms;
	
	netWmState = XInternAtom(display, "_NET_WM_STATE", True);
	netWmStateHidden = XInternAtom(display, "_NET_WM_STATE_HIDDEN", True);
	
	XGetWindowProperty(display, window, netWmState,
		0, 0x77777777,
		False, AnyPropertyType,
		&actual_type, &actual_format, &nitems, &bytes_after, (unsigned char**)&atoms);
	
	int retval = 0;
	
	for(int i = 0; i < nitems; ++i){
		retval = retval || atoms[i] == netWmStateHidden;
	}
	
	return retval;
}

/**
 * Получить имя окна
**/
char *get_window_name(Display *display, Window window){
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

  return name;
}

/**
 * Проверка, совпадает ли имя окна с needle
**/
int match_window_name(Display *display, Window window, const char *needle) {
  char *name = get_window_name(display, window);
  
  int retval = strstr(name, needle) != NULL;

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

/**
  * Скрыть/раскрыть окно
  * Возвращает состояние окна **после** вызова функции
  * 0 --- скрыто
  * 1 --- раскрыто
**/
int toggle_window(Display *display, Window window){
	if(!is_window_hidden(display, window)){
		XIconifyWindow(display, window, 0);
		return 0;
	}
	else{
		XMapWindow(display, window);
		return 1;
	}
}