#ifndef COMPILER_DJB2_H
#define COMPILER_DJB2_H

#include <stdint.h>

#define BUCKET_SIZE 211 /* to minimize hash collisions */
/* djb2 hash function. */
uint32_t hash(const char *s);

#endif
