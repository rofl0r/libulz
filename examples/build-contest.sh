#!/bin/bash
#musl-gcc console_test.c console.c -I /usr/local/musl/include/ncursesw/ -lpng -llept -lncursesw -ltinfow -lticw
gcc -Wall -Wextra -g console_test.c ../console.c -I.. -lncurses -o console_test
