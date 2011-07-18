#include "stringptrlist.h"
//RcB: DEP "stringptr.c"

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
					w += whit->size;
				}
			}
		}
		free(l);
	}
	return result;
}

stringptr* getlistitem(stringptrlist* l, size_t itemnumber) {
	if(!l || itemnumber >= l->size)
		return NULL;
	return (stringptr*) (((char*)l) + sizeof(stringptrlist) + (itemnumber * sizeof(stringptr)));
}

int setlistitem(stringptrlist* l, size_t itemnumber, char* buf, size_t buflen) {
	stringptr* item =getlistitem(l, itemnumber);
	if (!item) {
		return 0;
	}
	item->ptr = buf;
	item->size = buflen;
	return 1;
}	

stringptrlist* new_stringptrlist(size_t items) {
	stringptrlist* result = (stringptrlist*) malloc(sizeof(stringptrlist) + (sizeof(stringptr) * items));
	if(result) {
		result->size = 0;
		result->capa = items;
	}
	return result;
}

stringptrlist* resize_stringptrlist(stringptrlist* list, size_t items) {
	if(!list) return NULL;
	stringptrlist* result;
	if((result = (stringptrlist*) realloc(list, sizeof(stringptrlist) + (sizeof(stringptr) * items))))
		result->capa = items;
	return result;
}

int stringptrlist_add(stringptrlist** l, char* s, size_t len) {
	stringptrlist* b;
	if((*l)->size + 1 >= (*l)->capa) {
		if(!(b = resize_stringptrlist(*l, (*l)->capa * 2))) return 0;
		*l = b;
	}
	(*l)->size++;
	if(!(setlistitem(*l, (*l)->size - 1, s, len))) {
		(*l)->size--;
		return 0;
	}
	return 1;
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

// parses line of a textfile.
// it returns a list of stringptrs. however they have not to be freed separately
// since they're alloced together with the list.
// also, '\n' in the original buffer will be replaced with '\0'
stringptrlist* parselines(stringptr* buf) {
	return stringptr_splitc(buf, '\n');
}

/*
stringptrlist* listdir(char* dirname) {
	DIR *dirp;
	struct dirent *dp;
	size_t len;

	if (!(dirp = opendir(dirname))) return NULL;
	size_t vol = (4096 - sizeof(stringptrlist)) / sizeof(stringptr); // lets start with one heap page
	stringptrlist* result = new_stringptrlist(vol);
	do {
		if ((dp = readdir(dirp))) {
			result->size++;
			if(result->size > vol) {
				vol *= 2;
				result = resize_stringptrlist(result, vol);
				if(!result) return NULL;
			}
			len = strlen(dp->d_name);
			setlistitem(result, result->size-1, strdup_n(dp->d_name, len), len);
		}
	} while (dp);
	closedir(dirp);
	return result;
}

int stringptrlist_comparefunc(const void* aa, const void* bb) {
	const stringptr* a = (const stringptr*) aa; 
	const stringptr* b = (const stringptr*) bb;
	if(a->size && b->size) {
		if(*(a->ptr) > *(b->ptr)) return 1;
		else if (*(a->ptr) == *(b->ptr)) return 0;
		return -1;
	} else {
		if(a->size) return 1;
		if(b->size) return -1;
		return 0;
	}
}

void stringptrlist_sort(stringptrlist* l) {
	if(!l || !l->size) return;
	stringptr* i = getlistitem(l, 0);
	if(i) qsort((void*) i, l->size, sizeof(stringptr), stringptrlist_comparefunc);
}

*/
