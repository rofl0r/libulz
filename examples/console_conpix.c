//============================================================================
// Name        : conpix.c
// Author      : rofl0r
// Version     :
// Copyright   : GPL v2
// Description : a picture viewer for the console. uses xterm-256color mode.
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <leptonica/allheaders.h>

#include "../console.h"
//#include "strlib.h"

int main(int argc, char** argv) {
	char* filename;
	int scaleFullScreen = 0;
	console co;
	console* t = &co;
	int cx; int cy;
	int w, h;
	int iterX, iterY;
	float xfactor = 1.0f;
	float yfactor = 1.0f;
	float factor;
	struct Pix* pngfile;
	struct Pix* ping;
	struct Pix* palette;
	struct Pix* pix32;
	int i, ix, iy;
	
	for (i = 1; i<argc; i++) {
		if (strlen(argv[i]) > 1 && !memcmp(argv[i], "-f", 2)) 
			scaleFullScreen=1;
		else filename = argv[i];
	}

	if (access(filename, R_OK)) {
		puts("file not found!");
		puts("c0npix by rofl0r");
		puts("================");
		printf("arguments: %s [-f] somefile.[jpg|png|bmp|tiff]\n", argv[0]);
		puts("where -f means scale to fullscreen");
		puts("export TERM=xterm-256color before usage is recommended.");
		exit(1);
	}

	console_init(t);

	console_getsize(t, &cx, &cy);

	pngfile = pixRead(filename);

	pixGetDimensions(pngfile, &w, &h, NULL);

	if(scaleFullScreen) {
		xfactor = (cx * 1.0) / (w * 1.0);
		yfactor = (cy * 1.0) / (h * 1.0);
		ping = pixScale(pngfile, xfactor, yfactor);
	} else {
		if (cx *2 < w) xfactor = (cx * 1.0) / (w * 1.0);
		if (cy < h) yfactor = (cy * 1.0) / (h * 1.0);
		factor = (xfactor < yfactor) ? xfactor: yfactor;
		if (cx *2 < w || cy < h)
			ping = pixScale(pngfile, factor * 2.0, factor);
		else
			// scale to double width so the proportion of the font is correct
			ping = pixScale(pngfile, 2.0, 1.0 );
	}
	pixDestroy(&pngfile);
	
	palette = pixOctreeColorQuant(ping, 240, 1);
	
	if (palette == NULL) { puts("palette is nul"); goto finish_him; }

	pix32 = pixConvertTo32(palette);

	iterX = pix32->w;
	iterY = pix32->h;

	int* bufptr = (int*) pix32->data;
	if (bufptr == NULL) {
		puts("bufptr is null");
		goto finish_him;
	}

	for (iy = 0; iy < iterY; iy++){
		for (ix = 0; ix < iterX; ix++){
			console_setcolor(t, *((rgb_t*) bufptr), 0);
			console_gotoxy(t, ix, iy);
			console_addchar(t, ' ', 0);
			bufptr++;
		}
	}
	pixDestroy(&palette);
	pixDestroy(&pix32);

	console_refresh(t);
	
	finish_him:
	console_getkey(t);
	
	console_quit(t);

	return 0;
}
