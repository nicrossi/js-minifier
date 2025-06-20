#ifndef COMPILER_EXPRESSIONGENERATOR_H
#define COMPILER_EXPRESSIONGENERATOR_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/Logger.h"
#include "../../shared/Utils.h"

void initializeExpressionGeneratorModule();
void shutdownExpressionGeneratorModule();

void genExpression(Expression * expression);

#endif
