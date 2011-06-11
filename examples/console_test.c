#include <stdlib.h>
#include "console.h"

int main(void) {
	static const char japh[] = "just another perl/unix hacker";
	static const size_t jl = sizeof(japh);
	console co;
	console* c = &co;
	int w, h, x, y = 0;
	console_init(c);
	console_getsize(c, &w, &h);
	console_setcolor(c, RGB(80, 80, 80), 1);
	console_setcolor(c, RGB(200, 80, 200), 0);
	for(x = 0; x < w; x++) {
		console_gotoxy(c, x, y);
		console_addchar(c, ' ', 0);
	}
	console_setcolor(c, RGB(80, 80, 80), 0);
	console_setcolor(c, RGB(255, 255, 255), 1);
	for(y = 1; y < h - 1; y++) {
		for(x = 0; x < w; x++) {
			console_gotoxy(c, x, y);
			console_addchar(c, ACS_CKBOARD, 0);
		}
	}
	y = h - 1;
	console_setcolor(c, RGB(80, 80, 80), 1);
	console_setcolor(c, RGB(200, 80, 200), 0);
	for(x = 0; x < w; x++) {
		console_gotoxy(c, x, y);
		console_addchar(c, ' ', 0);
	}

	console_setcolor(c, RGB(255, 255, 80), 1);
	console_setcolor(c, RGB(0, 0, 200), 0);
	
	y = 15;

	console_gotoxy(c, 20, y++);
	console_printchar(c, ACS_ULCORNER, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_URCORNER, 0);
	
	console_gotoxy(c, 20, y++);
	console_printchar(c, ACS_VLINE, 0);
	for(x=0; x<8; x++)
		console_printchar(c, ' ', 0);
	console_printchar(c, ACS_VLINE, 0);
	
	console_gotoxy(c, 20, y++);
	console_printchar(c, ACS_LLCORNER, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_HLINE, 0);console_printchar(c, ACS_HLINE, 0);
	console_printchar(c, ACS_LRCORNER, 0);
	y++;
	console_gotoxy(c, 20, y++);
	console_printchar(c, ACS_RTEE, 0);console_printchar(c, ACS_LTEE, 0);
	console_printchar(c, ACS_BTEE, 0);console_printchar(c, ACS_TTEE, 0);
	console_printchar(c, ACS_PLUS, 0);console_printchar(c, ACS_S1, 0);
	console_printchar(c, ACS_S9, 0);console_printchar(c, ACS_DIAMOND, 0);
	
	console_gotoxy(c, 20, y++);
	console_printchar(c, ACS_CKBOARD, 0);console_printchar(c, ACS_DEGREE, 0);
	console_printchar(c, ACS_PLMINUS, 0);console_printchar(c, ACS_BULLET, 0);
	console_printchar(c, ACS_LARROW, 0);console_printchar(c, ACS_RARROW, 0);
	console_printchar(c, ACS_DARROW, 0);console_printchar(c, ACS_UARROW, 0);

	console_gotoxy(c, 20, y++);
	console_printchar(c, ACS_BOARD, 0);console_printchar(c, ACS_LANTERN, 0);
	console_printchar(c, ACS_BLOCK, 0);
	console_refresh(c);
	console_gotoxy(c, 20, y++);
	for(x = 0; x < jl; x++) {
		delay_output(rand() % 200 + 80);
		console_printchar(c, japh[x], 0);
		console_refresh(c);
		
	}	
	
	console_refresh(c);
	
	console_getkey(c);
	console_clear(c);
	console_quit(c);
	return 0;
}
