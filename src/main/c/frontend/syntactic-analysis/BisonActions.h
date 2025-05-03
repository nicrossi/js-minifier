#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "../../shared/ecalloc.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

StatementListItem * DeclarationStatementListItemSemanticAction(Declaration * declaration);
StatementList * EmptyStatementListSemanticAction();
Constant * IntegerConstantSemanticAction(const int value);
LexicalConst * LexicalConstSemanticAction(const char * string, Constant * constant);
Declaration * LexicalConstDeclarationSemanticAction(LexicalConst * lexicalConst);
Program * StatementListProgramSemanticAction(CompilerState * compilerState, StatementList * statementList);
StatementList * StatementListSemanticAction(StatementList * statementList, StatementListItem * statementListItem);
StatementListItem * StatementStatementListItemSemanticAction(Statement * statement);
#endif
