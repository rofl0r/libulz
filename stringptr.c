/*
 * author: rofl0r
 * 
 * License: LGPL 2.1+ with static linking exception
 * 
 */

#include "stringptr.h"
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

stringptr* new_string(size_t size) {
	stringptr* result = (stringptr*) malloc(sizeof(stringptr) + size + 1);
	if (result == NULL) return NULL;
	result->ptr = (char*)result + sizeof(stringptr);
	result->size = size;
	*((char*) (result->ptr + size)) = '\0';
	return result;
}

void free_string(stringptr* string) {
	free(string);
}

stringptr* copy_string(stringptr* s) {
	if(!s) return NULL;
	stringptr* result = new_string(s->size);
	memcpy(result->ptr, s->ptr, s->size);
	return result;
}

stringptr* stringptr_grow(stringptr*s, size_t newsize) {
	return realloc(s, newsize);
}

int streq(stringptr* a, stringptr* b) {
	if(!a && !b) return 1;
	if((a && !b) || (!a && b)) return 0;
	return (a->size == b->size && !memcmp(a->ptr, b->ptr, a->size));
}

size_t getfilesize(char* filename) {
	struct stat st;
	if (!stat(filename, &st)) {
		return st.st_size;
	} else return 0;
}

size_t writefile(char* filename, stringptr* buffer) {
	static const size_t chunksize = 64*1024;
	size_t bufpos = 0;
	size_t ret = 0;
	FILE* f = NULL;
	size_t togo;
	if(!filename || !buffer || !buffer->ptr || !(f = fopen(filename, "w"))) return 0;
	
	while(bufpos < buffer->size) {
		togo = buffer->size - bufpos;
		ret = fwrite(buffer->ptr+bufpos, 1, togo > chunksize ? chunksize : togo, f);
		if(!ret) {
			fputs(strerror(errno), stderr);
			break;
		}	
		bufpos += ret;
	}
	fclose(f);
	return bufpos;
}

stringptr* readfile(char* filename) {
	FILE* f;
	stringptr* buf;
	size_t size = getfilesize(filename);
	size_t bufpos = 0;
	size_t bread = 0;
	if(!size) return NULL;	
	f = fopen(filename, "r");
	if(!f) return NULL;
	buf = new_string(size);
	
	if (!buf) goto FEXIT;
	
	while(bufpos < size) {
		bread = fread(buf->ptr + bufpos, 1, 64*1024, f);
		bufpos += bread;		
		if(!bread) {
			fputs(strerror(errno), stderr);
			break;
		}	
	}	
	FEXIT:
	fclose(f);
	return buf;
}

stringptr* format(char* fmt, ...) {
	const size_t BUF_SIZE = 1000;
	stringptr* result = new_string(BUF_SIZE);
	if(!result || !fmt) return NULL;
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(result->ptr, BUF_SIZE, fmt, ap);
	va_end(ap);	
	return result;
}

// use like this: stringptr_concat(str1, str2, str3, NULL);
// returns a new string made up of all single strings passed...
stringptr* stringptr_concat(stringptr* self, ...) {
	size_t totalsize = self->size;
	stringptr* candidate = NULL;
	stringptr* result = NULL;
	char* dest;

	va_list ap;
	va_start(ap, self);
	while(1) {
		candidate = va_arg(ap, stringptr*);
		if(!candidate) break;
		totalsize += candidate->size;
	}
	va_end(ap);

	if ((result = new_string(totalsize))) {
		memcpy(result->ptr, self->ptr, self->size);
		dest = result->ptr + self->size;
		va_start(ap, self);
		while(1) {
			candidate = va_arg(ap, stringptr*);
			if(!candidate) break;
			memcpy(dest, candidate->ptr, candidate->size);
			dest += candidate->size;
		}
		va_end(ap);
	}
	return result;
}

stringptr* getlistitem(stringptrlist* l, size_t itemnumber) {
	if(!l || itemnumber >= l->size)
		return NULL;
	return (stringptr*) (((char*)l) + sizeof(stringptrlist) + (itemnumber * sizeof(stringptr)));
}

void setlistitem(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen) {
	stringptr* item =getlistitem(l, itemnumber);
	if (!item) {
		return;
	}	
	item->ptr = buf;
	item->size = buflen;
}	

stringptrlist* new_stringptrlist(size_t items) {
	stringptrlist* result = (stringptrlist*) malloc(sizeof(stringptrlist) + (sizeof(stringptr) * items));
	if(result)
		result->size = 0;
	return result;
}

stringptrlist* resize_stringptrlist(stringptrlist* list, size_t items) {
	if(!list) return NULL;
	return (stringptrlist*) realloc(list, sizeof(stringptrlist) + (sizeof(stringptr) * items));
}

// splits a stringptr into a list
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, delim in the original buffer will be replaced with '\0'
stringptrlist* stringptr_splitc(stringptr* buf, int delim) {
	size_t vol = (4096 - sizeof(stringptrlist)) / sizeof(stringptr); // lets start with one heap page
	size_t i, linestart;
	linestart = 0;
	stringptrlist* result = new_stringptrlist(vol);
	if(!result) return NULL;

	for(i=0;i<=buf->size;i++) {
		if(i == buf->size || buf->ptr[i] == delim) {			
			result->size++;
			if(result->size > vol) {
				vol *= 2;
				result = resize_stringptrlist(result, vol);
				if(!result) return NULL;
			}
			setlistitem(result, result->size-1, buf->ptr + linestart, i - linestart);
			linestart = i+1;
			buf->ptr[i] = 0;
		}
	}
	return result;
}

static inline int islb(char* p) {
	return *p == '\n' || *p == '\r';
}

// removes _all_ \r and \n 's from the tail of a stringptr. returns the count of chars removed.
size_t stringptr_chomp(stringptr* s) {
	if(!s) return 0;
	size_t res = 0;
	while(s->size && islb(s->ptr + (s->size - 1)) && stringptr_shiftleft(s, 1)) res++;
	return res;
}

char* stringptr_strdup(stringptr* s) {
	if(!s || !s->size) return NULL;
	char* result;
	if(!(result = malloc(s->size + 1))) return NULL;
	memcpy(result, s->ptr, s->size + 1);
	return result;
}

int stringptr_shiftleft(stringptr* s, size_t count) {
	if(!s) return 0;
	if(s->size >= count) {
		s->size -= count;
		s->ptr[s->size] = 0;
		return 1;
	}
	return 0;
}

int stringptr_shiftright(stringptr* s, size_t count) {
	if(!s) return 0;
	if(s->size >= count) {
		s->size -= count;
		s->ptr += count;
		return 1;
	}
	return 0;
}

int stringhere(stringptr* haystack, size_t bufpos, stringptr* needle) {
	if(needle->size <= haystack->size - bufpos) {
		if(!memcmp(needle->ptr, haystack->ptr + bufpos, needle->size))
			return 1;
	}
	return 0;
}

// splits a stringptr into a list
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, the first character of any occurence of delim in the original buffer will be replaced with '\0'
// the count of items will always be number of matches + 1
// i.e. if there's no match, the entire source buffer will be returned as first listitem.
stringptrlist* stringptr_splits(stringptr* buf, stringptr* delim) {
	size_t vol = (4096 - sizeof(stringptrlist)) / sizeof(stringptr); // lets start with one heap page
	size_t i, linestart;
	linestart = 0;
	stringptrlist* result = new_stringptrlist(vol);
	if(!result) return NULL;
	i = 0;
	while(i<=buf->size) {
		if(i == buf->size || stringhere(buf, i, delim)) {			
			result->size++;
			if(result->size > vol) {
				vol *= 2;
				result = resize_stringptrlist(result, vol);
				if(!result) return NULL;
			}
			setlistitem(result, result->size-1, buf->ptr + linestart, i - linestart);
			linestart = i + delim->size;
			buf->ptr[i] = 0;
			i += delim->size;
		} else
			i++;
	}
	return result;
}

stringptr* stringptr_replace(stringptr* buf, stringptr* what, stringptr* whit) {
	if(!buf || !what || !whit) return NULL;
	stringptrlist* l;
	stringptr* result = NULL, *temp;
	size_t i, w;
	if((l = stringptr_splits(buf, what))) {
		result = new_string(buf->size - ((l->size - 1) * what->size) + ((l->size - 1) * whit->size));
		if(!result) return NULL;
		w = 0;
		for(i = 0; i < l->size; i++) {
			if((temp = getlistitem(l, i))) {
				if(temp->size) {
					memcpy(result->ptr + w, temp->ptr, temp->size);
					w += temp->size;
				}
				if((i < l->size - 1) && whit->size) {
					memcpy(result->ptr + w, whit->ptr, whit->size);
					w+=whit->size;
				}
			}
		}
		free(l);
	}
	return result;
}

// parses line of a textfile.
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, '\n' in the original buffer will be replaced with '\0'
stringptrlist* parselines(stringptr* buf) {
	return stringptr_splitc(buf, '\n');
}

