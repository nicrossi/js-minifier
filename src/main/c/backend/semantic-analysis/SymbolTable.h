#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../../shared/ecalloc.h"
#include "../../shared/djb2.h"

typedef enum {
    SYM_LET,
    SYM_CONST,
    SYM_FUNC
} SymKind;

/* Metadata that the semantic analyser stores for each identifier. */
typedef struct {
    SymKind kind;
    char * minifiedName;
} SymbolInfo;

typedef struct SymbolTable SymbolTable;

/* Creates a new symbol table. */
SymbolTable * stCreate(void);

/* Destroys the symbol table and free all its resources */
void stDestroy(SymbolTable * st);

/* Enters a new scope, pushing a new frame onto the stack. */
void stEnterScope(SymbolTable * st);

/* Exits the current scope, popping the top frame from the stack. */
void stExitScope(SymbolTable * st);

/* Insert returns true on success, false when the name already exists
 * in the current (top-most) scope.
 * */
bool stInsert(SymbolTable * st, const char * name, SymKind kind);

/* Lookup searches from innermost scope outwards
 * returns NULL if not found.
 * */
const SymbolInfo * stLookup(const SymbolTable * st, const char * name);

/* Purges all zombie frames, freeing their resources.
 * This is called at the end of the compilation.
 * */
void stPurge(void);

#endif
