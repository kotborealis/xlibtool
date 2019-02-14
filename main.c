#include <stdio.h>

#include <string.h>

#include <X11/Xlib.h>

#include "xlibtool.h"

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
  printf("\t--is_hidden\tIs widnow hidden? (minified?)\n");
  printf("\t--toggle\tRaise window if hidden, iconify if not\n");
}

int main(int argc, char** argv){
  int retval = 0;

  int lower = 0;
  int iconify = 0;
  int raise = 0;
  int focus = 0;
  int map = 0;
  int unmap = 0;
  int is_hidden = 0;
  int toggle = 0;

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
        is_hidden += strcmp(_, "is_hidden") == 0;
        toggle += strcmp(_, "toggle") == 0;
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

	if(is_hidden)
		printf("is_hidden: %s\n", 
			is_window_hidden(display, window) ? "True" : "False");
			
	if(toggle)
		printf("is_hidden: %s\n", 
			!toggle_window(display, window) ? "True" : "False");

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