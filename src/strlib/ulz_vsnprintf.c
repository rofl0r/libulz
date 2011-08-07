#include <stdarg.h>
#include "../../include/vario.h"
#include "../../include/strlib.h"

// lightweight sprintf replacement. supports width via %.2x and all int types, and %s, and %% for an escaped %
// returns the number of bytes written to the target. should behave exactly as posix sprintf except %x also returns hex decimals in uppercase.
// destsize is treated like in the original. it will leave space for a trailing 0,
// if NULL is passed as dest it will return the number of bytes it would haven written if a sufficiently large buffer was provided.
ssize_t ulz_vsnprintf(char* dest, size_t destsize, const char* format, va_list ap) {
	char *ins, *a, *o = dest, *omax = dest ? dest + destsize - 1: NULL;
	char cbuf[32];
	uint64_t inn;
	size_t i,j, width = 0;
	ssize_t res = 0;
	int nflags = 0;
	int lng, lflag; // only required in case 'l'
	unsigned int base = 10;
	for(a = (char*)format; *a; a++) {
		if(*a == '%') {
			checkmod:
			++a;
			switch(*a) {
				case '%':
					goto inschar;
				case '.':
					i = 0;
					a++;
					while(isNumber(a + i)) i++;
					if(!i) {
						fail:
						return -1;
					}
					width = 0;
					for(j = 0; j < i; j++) width += (a[j] - '0') * (j ? (10 * (i - 1 - j)) : 1);
					a += i - 1;
					goto checkmod;
				case 's':
					ins = va_arg(ap, char*);
					strmove:
					if(!ins) goto fail;
					while(*ins) {
						if(o < omax) {
							*o = *ins;
							o++;
							res++;
						} else if (!o) res++;
						ins++;
					}
					break;
				case 'd':
					nflags = NTS_SIGNED_TYPE;
					intstart:
					inn = va_arg(ap, int);
					nconv:
					if(width) nflags |= NTS_PAD;
					ins = numberToString(inn, base, cbuf, width, nflags);
					width = 0;
					base = 10;
					goto strmove;
					break;
				case 'l': case 'z':
					lng = 0;
					lflag = a[0] == 'l';
					if(lflag) {
						if(a[1] == 'l') {
							lng = 1; a++;
						} else lng = 0;
					}
					a++;
					nflags = (a[0] == 'd') ? NTS_SIGNED_TYPE : 0;
					if(!nflags && a[0] != 'u') goto fail; // TODO still required ? doesnt look like it
					if(lflag) {
						if(!lng)
							inn = va_arg(ap, unsigned long);
						else
							inn = va_arg(ap, unsigned long long);
					} else 
						inn = va_arg(ap, size_t);
					goto nconv;
				case 'p':
					base = 16; nflags = 0;
					inn = va_arg(ap, size_t);
					goto nconv;
				case 'u':
					inn = va_arg(ap, unsigned);
					nflags = 0;
					goto nconv;
				case 'x': case 'X':
					base = 16;
					nflags = (*a == 'x') ? NTS_LOWERCASE_CHARS : 0;
					goto intstart;
				default:
					goto fail;
			}
		} else {
			inschar:
			if(o < omax) {
				*o = *a;
				o++;
				res++;
			} else if(!o) res++;
		}
	}
	if(o) o[0] = 0;
	return res;
}
