#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Utils.h"
#include "../../backend/code-generation/ExpressionGenerator.h"
#include <stdarg.h>
#include <stdio.h>

// I've never liked macros, but here we are...
// Force indent 0 in output()
#define EMIT(fmt, ...) output(0, fmt, ##__VA_ARGS__)
void output(const unsigned int indentationLevel, const char * const format, ...);

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/* Generates the final output using the current compiler state. */
void generate(CompilerState * compilerState);

#endif
