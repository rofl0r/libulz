#!/bin/bash
#musl-gcc console_conpix.c ../console.c -I.. -I/usr/local/musl/include/ncursesw/ -llept -lz -lpng -ltiff -ljpeg -lncursesw -ltinfow -lticw
gcc -Wall -Wextra -g console_conpix.c ../console.c -I.. -lncurses -llept -o console_conpix
