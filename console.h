/*
 * console.h
 *
 * Created on: 29.11.2010
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 * 
 * interesting attr_t's:
 * 
 * WA_ALTCHARSET  Alternate character set
 * WA_BLINK       Blinking
 * WA_BOLD        Extra bright or bold
 * WA_DIM         Half bright
 * WA_HORIZONTAL  Horizontal highlight
 * WA_INVIS       Invisible
 * WA_LEFT        Left highlight
 * WA_LOW         Low highlight
 * WA_PROTECT     Protected
 * WA_REVERSE     Reverse video
 * WA_RIGHT       Right highlight
 * WA_STANDOUT    Best highlighting mode of the terminal
 * WA_TOP         Top highlight
 * WA_UNDERLINE   Underlining
 * WA_VERTICAL    Vertical highlight
 * 
 * interesting special chars:
 * 
 * 
 * ACS_ULCORNER	WACS_ULCORNER 	+ 	upper left-hand corner 
 * ACS_LLCORNER	WACS_LLCORNER 	+ 	lower left-hand corner 
 * ACS_URCORNER	WACS_URCORNER 	+ 	upper right-hand corner 
 * ACS_LRCORNER	WACS_LRCORNER 	+ 	lower right-hand corner 
 * ACS_RTEE 	WACS_RTEE 	+ 	right tee 
 * ACS_LTEE 	WACS_LTEE 	+ 	left tee 
 * ACS_BTEE 	WACS_BTEE 	+ 	bottom tee 
 * ACS_TTEE 	WACS_TTEE 	+ 	top tee 
 * ACS_HLINE 	WACS_HLINE 	- 	horizontal line 
 * ACS_VLINE 	WACS_VLINE 	| 	vertical line 
 * ACS_PLUS 	WACS_PLUS 	+ 	plus 
 * ACS_S1 	WACS_S1 	- 	scan line 1 
 * ACS_S9 	WACS_S9 	_ 	scan line 9 
 * ACS_DIAMOND 	WACS_DIAMOND 	+ 	diamond 
 * ACS_CKBOARD 	WACS_CKBOARD 	: 	checker board (stipple) 
 * ACS_DEGREE 	WACS_DEGREE 	' 	degree symbol 
 * ACS_PLMINUS 	WACS_PLMINUS 	# 	plus/minus 
 * ACS_BULLET 	WACS_BULLET 	o 	bullet 
 * ACS_LARROW 	WACS_LARROW 	< 	arrow pointing left 
 * ACS_RARROW 	WACS_RARROW 	> 	arrow pointing right 
 * ACS_DARROW 	WACS_DARROW 	v 	arrow pointing down 
 * ACS_UARROW 	WACS_UARROW 	^ 	arrow pointing up 
 * ACS_BOARD 	WACS_BOARD 	# 	board of squares 
 * ACS_LANTERN 	WACS_LANTERN 	# 	lantern symbol 
 * ACS_BLOCK 	WACS_BLOCK 	# 	solid square block
 * 
 * 
 */

#ifndef CONSOLEWINDOW_H_
#define CONSOLEWINDOW_H_

#include <stdint.h>
#include <curses.h>

#include "rgb.h"

#define CONSOLE_COLORPAIRCOUNT 256

typedef struct {
	int fgcol;
	int bgcol;
} Colorpair;

typedef struct {
	int hasColors;
	int canChangeColors;
	int x;
	int y;
	int maxcolor;

	attr_t lastattr;
	Colorpair lastused;
	Colorpair active;
	Colorpair termPairs[CONSOLE_COLORPAIRCOUNT];
	rgb_t colors[CONSOLE_COLORPAIRCOUNT];
	
	rgb_t org_colors[CONSOLE_COLORPAIRCOUNT];
	short int org_fgcolors[CONSOLE_COLORPAIRCOUNT];
	short int org_bgcolors[CONSOLE_COLORPAIRCOUNT];
} console;

void console_init(console* self);
void console_quit(console* self);
int console_setcolor(console* self, rgb_t mycolor, int fg);
void console_initoutput(console* self);
void console_getsize(console* self, int* x, int* y);
void console_gotoxy(console* self, int x, int y);
void console_addchar(console* self, int c, unsigned int attributes);
void console_printchar(console* self, int c, unsigned int attributes);
void console_printxy (console* self, int x, int y, char* text);
int console_getkey(console* self);
void console_sleep(console* self, int ms);
void console_refresh(console* self);
void console_clear(console* self);


#endif /* CONSOLEWINDOW_H_ */