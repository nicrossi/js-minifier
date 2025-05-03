#ifndef COMPILER_ECALLOC_H
#define COMPILER_ECALLOC_H

#include <stdlib.h>
#include <stdio.h>

/*
 * This  function do the same as calloc(), but if there is	an error
 * it will print a message with	errx(3), and exit.
 * */
void * ecalloc(size_t nmemb, size_t size);

#endif //COMPILER_ECALLOC_H
