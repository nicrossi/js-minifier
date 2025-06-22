#include "djb2.h"

/**
 * djb2 - Daniel J. Bernstein hash function.
 * @see http://www.cse.yorku.ca/~oz/hash.html
 * */
uint32_t hash(const char *s) {
    uint32_t h = 5381u;
    for (; *s; ++s)
        h = (h * 33) ^ (uint8_t) (*s);

    return h % BUCKET_SIZE;
}
