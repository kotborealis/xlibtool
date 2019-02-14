main: libxlibtool.a main.c
	gcc -std=c99 main.c libxlibtool.a -o xlibtool -L/usr/X11/lib -lX11
	
xlibtool.o: xlibtool.c xlibtool.h
	gcc -std=c99 xlibtool.c -c -o xlibtool.o
	
libxlibtool.a: xlibtool.o
	ar rcs libxlibtool.a xlibtool.o
	
.PHONY: clean
clean:
	rm xlibtool xlibtool.o libxlibtool.a