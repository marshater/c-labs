#!/bin/bash
ARCH=arm \
CC=arm-linux-gnueabi-gcc ./configure \
GLIB_CFLAGS="-I/tmp/build/include/glib-2.0 -I/tmp/build/lib/glib-2.0/include" \
GLIB_LIBS="-L/tmp/build/lib -lglib-2.0" \
GMODULE_CFLAGS="-I/tmp/build/include/glib-2.0 -I/tmp/build/lib/glib-2.0/include" \
GMODULE_LIBS="-L//tmp/build/lib/glib-2.0 -lgmodule-2.0" \
--host=arm-linux-gnueabi \
--prefix=/tmp/build/ \
--enable-static=yes \
--with-screen=ncurses \
--with-ncurses-includes="/tmp/build/include" \
--with-ncurses-libs="/tmp/build/lib"
