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
Expression * AssignmentExpressionSemanticAction(Expression * left, Expression * right);
StatementList * BlockSemanticAction(StatementList * statementList);
Statement * BlockStatementSemanticAction(StatementList * statementList);
Expression * ChainedAssignmentSemanticAction(Expression * left, Expression * right);
Expression * CommaExpressionSemanticAction(Expression * expression, Expression * assignmentExpression);
LexicalDeclaration * CreateLexicalDeclarationSemanticAction(LexicalDeclarationType type, VariableDeclaratorList * variableDeclaratorList);
StatementListItem * DeclarationStatementListItemSemanticAction(Declaration * declaration);
Expression * EmptyExpressionSemanticAction();
ForInitializer * EmptyForInitSemanticAction();
StatementList * EmptyBlockSemanticAction();
StatementList * EmptyStatementListSemanticAction();
Expression * EqualityExpressionSemanticAction(Expression * left, Expression * right);
ForInitializer * ExpressionForInitSemanticAction(Expression * expression);
Statement * ExpressionStatementSemanticAction(Expression * expression);
ForStatement * ForIterationSemanticAction(ForInitializer * initializer, Expression * condition, Expression * increment, Statement * body);
Statement * ForStatementSemanticAction(ForStatement * forStatement);
Expression * IdentifierExpressionSemanticAction(const char * identifier);
IfStatement * IfSemanticAction(Expression * condition, Statement * thenStatement, Statement * elseStatement);
Statement * IfStatementSemanticAction(IfStatement * ifStatement);
Expression * IntegerExpressionSemanticAction(int value);
ForInitializer * LexicalDeclarationForInitSemanticAction(LexicalDeclaration * forLexicalDeclaration);
Declaration * LexicalDeclarationSemanticAction(LexicalDeclaration * lexicalConst);
Program * StatementListProgramSemanticAction(CompilerState * compilerState, StatementList * statementList);
StatementList * StatementListSemanticAction(StatementList * statementList, StatementListItem * statementListItem);
StatementListItem * StatementStatementListItemSemanticAction(Statement * statement);
Expression * StringExpressionSemanticAction(const char * s);
VariableDeclaratorList * VariableDeclaratorListSemanticAction(VariableDeclarator * variableDeclarator);
VariableDeclarator * VariableDeclaratorSemanticAction(const char * identifier, Expression * initializer);
#endif
