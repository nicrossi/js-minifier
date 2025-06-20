#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H

/* Returns how many elements are in the array. */
/* MUST be used on real arrays, not pointers. */
#define ARRAY_LEN(a)  (sizeof(a) / sizeof((a)[0]))

#endif
