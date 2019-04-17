#ifndef VARIO_H
#define VARIO_H

#include "stringptr_struct.h"

typedef enum {
	VT_STRINGPTR,
	VT_CHAR_STAR,
	VT_CONST_CHAR_STAR,
	VT_INT,
	VT_UNSIGNED,
	VT_LONG,
	VT_ULONG,
} variotype;

typedef struct {
	void* ptr;
	variotype type;
} vario;

#define VNIL ((vario*) 0)

#define VARIO_STRINGPTR_PTR(X) (&(stringptr*){X})
#define VARIO_CHAR_STAR_PTR(X) (&(char*){X})
#define VARIO_CONST_CHAR_STAR_PTR(X) (&(const char*){X})
#define VARIO_INT_PTR(X) (&(int){X})
#define VARIO_UNSIGNED_PTR(X) (&(unsigned){X})
#define VARIO_LONG_PTR(X) (&(long){X})
#define VARIO_ULONG_PTR(X) (&(unsigned long){X})

#define VARIS(X) &(vario){VARIO_STRINGPTR_PTR(X), VT_STRINGPTR}
#define VARISL(X) &(vario){VARIO_STRINGPTR_PTR((SPLITERAL(X))), VT_STRINGPTR}
#define VARIC(X) &(vario){VARIO_CHAR_STAR_PTR(X), VT_CHAR_STAR}
#define VARICC(X) &(vario){VARIO_CONST_CHAR_STAR_PTR(X), VT_CONST_CHAR_STAR}
#define VARII(X) &(vario){VARIO_INT_PTR(X), VT_INT}
#define VARIU(X) &(vario){VARIO_UNSIGNED_PTR(X), VT_UNSIGNED}
#define VARIL(X) &(vario){VARIO_LONG_PTR(X), VT_LONG}
#define VARIUL(X) &(vario){VARIO_ULONG_PTR(X), VT_ULONG}

#endif
