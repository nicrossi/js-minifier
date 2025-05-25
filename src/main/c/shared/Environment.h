#ifndef ENVIRONMENT_HEADER
#define ENVIRONMENT_HEADER

#include "Type.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static const char *outputFileName = NULL;

/**
 * Analog to "getStringOrDefault", but returning true if the string is exactly
 * "true"; false otherwise. The default value is only used when the variable
 * is undefined.
 */
const boolean getBooleanOrDefault(const char * name, const boolean defaultValue);

/**
 * Gets the value of an environment variable by name, or returns a default
 * value if the variable is undefined.
 * 
 * @see https://cplusplus.com/reference/cstdlib/getenv/
 */
const char * getStringOrDefault(const char * name, const char * defaultValue);

/**
 * Gets the output stream to be used by the program, which is either the
 * standard output or a file specified by the "OUTPUT_FILE" argument.
 *
 * @see https://cplusplus.com/reference/cstdio/fopen/
 */
FILE * getOutputStream();

/**
 * Sets the output file name to be used by the program.
 */
void setOutputFileName(const char *filename);
#endif
