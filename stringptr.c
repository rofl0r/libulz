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

int streq(stringptr* a, stringptr* b) {
	if(!a && !b) return 1;
	if((a && !b) || (!a && b)) return 0;
	return (a->size == b->size && !strcmp(a->ptr,b->ptr));
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

// parses line of a textfile.
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, '\n' in the original buffer will be replaced with '\0'
stringptrlist* parselines(stringptr* buf){
	size_t vol = (4096 - sizeof(stringptrlist)) / sizeof(stringptr); // lets start with one heap page
	size_t i, linestart;	
	linestart = 0;	
	stringptrlist* result = new_stringptrlist(vol);
	if(!result) return NULL;

	for(i=0;i<=buf->size;i++) {
		if(i == buf->size || buf->ptr[i] == '\n') {			
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

