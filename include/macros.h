#ifndef MACROS_H
#define MACROS_H

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#ifndef WORD_SIZE
#define WORD_SIZE (sizeof(void*))
#endif

#define INT3 __asm__("int3")

#endif

