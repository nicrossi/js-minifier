#ifndef COMPILER_SEMANTICANALYZER_H
#define COMPILER_SEMANTICANALYZER_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Utils.h"
#include "SymbolTable.h"
#include <assert.h>
#include <stddef.h>
#include <math.h>


void initializeSemanticAnalyzerModule(void);
void shutdownSemanticAnalyzerModule(void);

bool validateProgram(const Program * program);

#endif
