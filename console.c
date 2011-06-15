/*
 *
 * Created on: 29.11.2010
 * 
 * 
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 * 
 * 
 * library for sane 256 color handling in xterm
 * 
 * without manual bookkeeping for colorpairs
 * 
 * 
 */

#include "console.h"
#include <unistd.h>

#define MIN_COLORPAIR_NUMBER 1
#define MIN_COLOR_NUMBER 0

//#define CONSOLE_DEBUG
static rgb_t invalid_color = RGB(0,0,0);

#ifdef CONSOLE_DEBUG	
static FILE* dbg = NULL;
#endif

void console_savecolors(console* self);
void console_restorecolors(console* self);
int console_setcursescolor(console* self, int colornumber, rgb_t color);
int console_setcolorpair(console* self, int pair, int fgcol, int bgcol);
int console_usecolorpair(console* self, int pair);



static inline int console_fromthousand(int in) {
	return in == 0 ? 0 : in == 1000 ? 255 : (in * 1000 * 1000) / 3921568;
}

static inline int console_tothousand(int in) {
	return in == 0 ? 0 : in == 255 ? 1000 : (in * 3921568) / (1000 * 1000);
}

static inline void console_inittables(console* self) {
	int i;
	for (i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		self->colors[i] = invalid_color;
		self->termPairs[i].fgcol = -1;
		self->termPairs[i].bgcol = -1;
	}
}

void console_init(console* self) {
	invalid_color.a = 255;
	self->active.fgcol = -1;
	self->active.fgcol = -1;
	
	self->lastattr = 0;
	
	console_inittables(self);
	
	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	
	self->hasColors = has_colors();
	self->canChangeColors = self->hasColors ? can_change_color() : 0;
	if (self->hasColors) start_color();

	if (self->canChangeColors)
		console_savecolors(self);
	
	self->lastattr = 0;
	
	self->maxcolor = 0;
	
	self->lastused.fgcol = -1;
	self->lastused.bgcol = -1;
	
#ifdef CONSOLE_DEBUG	
	dbg = fopen("console.log", "w");
#endif	
	
}

void console_quit(console* self) {
	clear();
	refresh();
	if (self->canChangeColors) console_restorecolors(self);
	endwin();
#ifdef CONSOLE_DEBUG		
	fclose(dbg);
#endif	
}

void console_savecolors(console* self) {
	short int i;
	short int r,g,b;
	short int fg, bg;
	for (i = MIN_COLOR_NUMBER; i < CONSOLE_COLORPAIRCOUNT; i++) {
		color_content(i, &r, &g, &b);
		self->org_colors[i] = RGB(console_fromthousand(r), console_fromthousand(g), console_fromthousand(b));
	}
	for (i = MIN_COLORPAIR_NUMBER; i < CONSOLE_COLORPAIRCOUNT; i++) {
		pair_content(i, &fg, &bg);
		self->org_fgcolors[i] = fg;
		self->org_bgcolors[i] = bg;
	}
}

void console_restorecolors(console* self) {
	int i;
	for (i = MIN_COLOR_NUMBER; i < self->maxcolor + MIN_COLOR_NUMBER; i++) {
		init_color(i, 
			console_tothousand(self->org_colors[i].r), 
			console_tothousand(self->org_colors[i].g),
			console_tothousand(self->org_colors[i].b)
		);
		//console_setcursescolor(self, i, self->org_colors[i]);
	}
	for (i = MIN_COLORPAIR_NUMBER; i < self->maxcolor + MIN_COLORPAIR_NUMBER; i++) {
		init_pair(i, self->org_fgcolors[i], self->org_fgcolors[i]);
	}
}

// needs color additionally to be used by restorecolors
int console_setcursescolor(console* self, int colornumber, rgb_t color) {
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcursescolor: %d (%d, %d, %d)\n", colornumber, color.r, color.g, color.b);
#endif
	
	if(colornumber >= CONSOLE_COLORPAIRCOUNT) return 0;
	
	// we use rgb values in the range 0-0xFF, while ncurses max is 1000
	if(!self->canChangeColors) return 0;

	int nr = console_tothousand(color.r);
	int ng = console_tothousand(color.g);
	int nb = console_tothousand(color.b);
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "init_color: %d (%d, %d, %d)\n", colornumber+1, nr, ng, nb);
#endif	
	
	return init_color(colornumber+MIN_COLOR_NUMBER, nr, ng, nb) != FALSE;
}

int console_setcolor(console* self, rgb_t mycolor, int fg) {
	int i;
	int* which = fg ? &self->active.fgcol : &self->active.bgcol;
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcolor: (%d, %d, %d), fg: %d\n", mycolor.r, mycolor.g, mycolor.b, fg);
#endif
	
	
	// see if it's the actual color...
	if (*which >= 0) {
		if (self->colors[*which].asInt == mycolor.asInt) return 1;
	}

	// this (c|sh)ould be optimized by using a hashmap
	for (i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		if (self->colors[i].asInt == invalid_color.asInt) {
				self->colors[i] = mycolor;
				console_setcursescolor(self, i, mycolor);
				if (i > self->maxcolor) self->maxcolor = i;
				found:
				*which = i;
#ifdef CONSOLE_DEBUG
				if(dbg) fprintf(dbg, "found at: %d\n", i);
#endif
				
				return 1;
		} else if (self->colors[i].asInt == mycolor.asInt) 
			goto found;
	}
	return 0; // "could not set color");
}

// sends the right "colorpair" to ncurses
void console_initoutput(console* self) {
	int i;
	if (self->active.fgcol == -1) console_setcolor(self, RGB(0xFF, 0xFF, 0xFF), 1);
	if (self->active.bgcol == -1) console_setcolor(self, RGB(0, 0, 0), 0);
	if(self->lastused.fgcol == self->active.fgcol && self->lastused.bgcol == self->active.bgcol)
		return;
	
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "initoutput: with fg: %d, bg: %d\n", self->active.fgcol, self->active.bgcol);
#endif
	
	for(i = 0; i < CONSOLE_COLORPAIRCOUNT; i++) {
		if(self->termPairs[i].fgcol == self->active.fgcol) {
				if (self->termPairs[i].bgcol != self->active.bgcol)
					continue;
				else {
					console_usecolorpair(self, i);
					return;
				}
		} else if (self->termPairs[i].fgcol == -1) {
				console_setcolorpair(self, i, self->active.fgcol, self->active.bgcol);
				console_usecolorpair(self, i);
				return;
		}
	}
	return; // "colorpair not found");
}

int console_setcolorpair(console* self, int pair, int fgcol, int bgcol) {
	if(fgcol >= CONSOLE_COLORPAIRCOUNT || bgcol >= CONSOLE_COLORPAIRCOUNT) return 0; // "color pair is out of index");
	if (!self->hasColors) return 0;
#ifdef CONSOLE_DEBUG
	if(dbg) fprintf(dbg, "setcolorpair: %d (fg: %d, bg: %d)\n", pair, fgcol, bgcol);
#endif
	
	self->termPairs[pair].fgcol = fgcol;
	self->termPairs[pair].bgcol = bgcol;
	return init_pair(pair+MIN_COLORPAIR_NUMBER, fgcol+MIN_COLOR_NUMBER, bgcol+MIN_COLOR_NUMBER) != FALSE;
}

int console_usecolorpair(console* self, int pair) {
	if(pair >= CONSOLE_COLORPAIRCOUNT) return 0;
	if (!self->hasColors) return 0;
	self->lastused.fgcol = self->active.fgcol;
	self->lastused.bgcol = self->active.bgcol;
	
	//if (self->lastattr) wattr_off(stdscr,self->lastattr,NULL);
	self->lastattr = COLOR_PAIR(pair + MIN_COLORPAIR_NUMBER);
	color_set(pair + MIN_COLORPAIR_NUMBER, NULL);
	//wattr_on(stdscr, self->lastattr, NULL);
	return 1;
}

void console_getsize(console* self, int* x, int* y){
	(void)self;
	if(stdscr) {
		*x = stdscr->_maxx + 1;
		*y = stdscr->_maxy + 1;
	} else { *y = -1; *x = -1; }
}

void console_gotoxy(console* self, int x, int y) {
	move(y, x);
	self->x = x;
	self->y = y;
}

// print a char at current location
void console_addchar(console* self, int c, unsigned int attributes) {
	console_initoutput(self);
	waddch(stdscr, c | attributes | self->lastattr);
	//waddch(stdscr, c | attributes);
}

// prints a char and advances cursor
void console_printchar(console* self, int c, unsigned int attributes) {
	int newx = self->x == stdscr->_maxx ? 1 : self->x + 1;
	int newy = self->x == stdscr->_maxx ? self->y + 1 : self->y;
	console_addchar(self, c, attributes);
	console_gotoxy(self, newx, newy);
}	


void console_printxy (console* self, int x, int y, char* text) {
	console_initoutput(self);
	mvprintw(x, y, "%s", text, 0);
}

int console_getkey(console* self) {
	(void)self;
	return wgetch(stdscr);
}

void console_sleep(console* self, int ms) {
	(void)self;
	napms(ms);
}

void console_refresh(console* self) {
	(void)self;
	refresh();
}

void console_clear(console* self) {
	(void)self;
	clear();
}