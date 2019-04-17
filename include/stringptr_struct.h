#ifndef STRINGPTR_STRUCT_H
#define STRINGPTR_STRUCT_H

#include <stddef.h>

typedef struct {
        char *ptr;
        size_t size;
} stringptr;

#define SPNIL ((stringptr*) 0)
#define SP_INIT(X, Y) {X, Y}
#define SPLITERAL(X) &(stringptr){(X), (sizeof((X)) - 1)}
#define SPINITIALIZER(X) {X, sizeof(X) - 1}
#define SPMAKE(X, Y) &(stringptr){(X), (Y)}

#endif
