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
VariableDeclaratorList * AppendVariableDeclaratorListSemanticAction(VariableDeclaratorList * list, VariableDeclarator * variableDeclarator);
Expression * AssignmentExpressionSemanticAction(VariableDeclarator * variableDeclarator);
StatementList * BlockSemanticAction(StatementList * statementList);
Statement * BlockStatementSemanticAction(StatementList * statementList);
Expression * ChainedAssignmentSemanticAction(Expression * left, Expression * right);
Expression * CommaExpressionSemanticAction(Expression * expression, Expression * assignmentExpression);
StatementListItem * DeclarationStatementListItemSemanticAction(Declaration * declaration);
StatementList * EmptyBlockSemanticAction();
StatementList * EmptyStatementListSemanticAction();
Expression * EqualityExpressionSemanticAction(Expression * left, Expression * right);
Statement * ExpressionStatementSemanticAction(Expression * expression);
Expression * IdentifierExpressionSemanticAction(const char * identifier);
IfStatement * IfSemanticAction(Expression * condition, Statement * thenStatement, Statement * elseStatement);
Statement * IfStatementSemanticAction(IfStatement * ifStatement);
Expression * IntegerExpressionSemanticAction(int value);
LexicalDeclaration * CreateLexicalDeclarationSemanticAction(LexicalDeclarationType type, VariableDeclaratorList * variableDeclaratorList);
Declaration * LexicalDeclarationSemanticAction(LexicalDeclaration * lexicalConst);
Program * StatementListProgramSemanticAction(CompilerState * compilerState, StatementList * statementList);
StatementList * StatementListSemanticAction(StatementList * statementList, StatementListItem * statementListItem);
StatementListItem * StatementStatementListItemSemanticAction(Statement * statement);
VariableDeclaratorList * VariableDeclaratorListSemanticAction(VariableDeclarator * variableDeclarator);
VariableDeclarator * VariableDeclaratorSemanticAction(const char * identifier, Expression * initializer);
#endif
