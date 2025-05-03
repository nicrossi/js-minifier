#include "ecalloc.h"

void * ecalloc(size_t nmemb, size_t size) {
    void * p = calloc(nmemb, size);
    if (p == NULL) {
        fprintf(stderr, "calloc: failed to allocate %zu bytes\n", nmemb * size);
        exit(EXIT_FAILURE);
    }
    return p;
}