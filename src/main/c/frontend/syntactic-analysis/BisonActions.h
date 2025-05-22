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
ArgumentList * AppendArgumentListSemanticAction(ArgumentList * argumentList, Expression * argument);
VariableDeclaratorList * AppendParameterListSemanticAction(VariableDeclaratorList * list, VariableDeclarator * parameter);
ArgumentList * ArgumentListSemanticAction(Expression * argument);
Expression * BooleanExpressionSemanticAction(bool v);
Expression * AssignmentExpressionSemanticAction(Expression * left, Expression * right);
StatementList * BlockSemanticAction(StatementList * statementList);
Statement * BreakStatementSemanticAction();
Statement * BlockStatementSemanticAction(StatementList * statementList);
Expression * CallExpressionSemanticAction(Expression * callee, ArgumentList * argumentList);
CatchClause * CatchClauseSemanticAction(const char * identifier, StatementList * block);
CatchClause * CatchClauseNoParamSemanticAction(StatementList * catchBlock);
Expression * ChainedAssignmentSemanticAction(Expression * left, Expression * right);
Expression * CommaExpressionSemanticAction(Expression * expression, Expression * assignmentExpression);
Statement * ContinueStatementSemanticAction();
LexicalDeclaration * CreateLexicalDeclarationSemanticAction(LexicalDeclarationType type, VariableDeclaratorList * variableDeclaratorList);
StatementListItem * DeclarationStatementListItemSemanticAction(Declaration * declaration);
WhileStatement * DoWhileSemanticAction(Statement * body, Expression * cond);
Statement * DoWhileStatementSemanticAction(WhileStatement * ws);
ArgumentList * EmptyArgumentListSemanticAction();
StatementList * EmptyBlockSemanticAction();
Expression * EmptyExpressionSemanticAction();
ForInitializer * EmptyForInitSemanticAction();
VariableDeclaratorList * EmptyParameterListSemanticAction();
StatementList * EmptyStatementListSemanticAction();
Expression * BinaryExpressionSemanticAction(Expression * left, Expression * right, ExpressionType type);
ForInitializer * ExpressionForInitSemanticAction(Expression * expression);
Statement * ExpressionStatementSemanticAction(Expression * expression);
FinallyClause * FinallyClauseSemanticAction(StatementList * finallyBlock);
Statement * ThrowStatementSemanticAction(Expression * expression);
ForStatement * ForIterationSemanticAction(ForInitializer * initializer, Expression * condition, Expression * increment, Statement * body);
Statement * ForStatementSemanticAction(ForStatement * forStatement);
Declaration * FunctionDeclarationSemanticAction(FunctionDeclaration * functionDeclaration);
FunctionDeclaration * FunctionSemanticAction(const char * identifier, VariableDeclaratorList * parameterList, StatementList * body);
Expression * IdentifierExpressionSemanticAction(const char * identifier);
IfStatement * IfSemanticAction(Expression * condition, Statement * thenStatement, Statement * elseStatement);
Statement * IfStatementSemanticAction(IfStatement * ifStatement);
Expression * IntegerExpressionSemanticAction(int value);
ForInitializer * LexicalDeclarationForInitSemanticAction(LexicalDeclaration * forLexicalDeclaration);
Declaration * LexicalDeclarationSemanticAction(LexicalDeclaration * lexicalConst);
Expression * NewExpressionSemanticAction(Expression * exp, ArgumentList * args);
Expression * OptionalExpression(Expression * expression);
VariableDeclaratorList * ParameterListSemanticAction(const char * parameter);
Expression * ParenthesisExpressionSemanticAction(Expression * expression);
Statement * ReturnStatementSemanticAction(Expression * expression);
Program * StatementListProgramSemanticAction(CompilerState * compilerState, StatementList * statementList);
StatementList * StatementListSemanticAction(StatementList * statementList, StatementListItem * statementListItem);
StatementListItem * StatementStatementListItemSemanticAction(Statement * statement);
Expression * StringExpressionSemanticAction(const char * s);
TryStatement * TryCatchSemanticAction(StatementList * tryBlock, CatchClause * catchClause);
TryStatement * TryFinallySemanticAction(StatementList * tryBlock, FinallyClause * finallyClause);
TryStatement * TryCatchFinallySemanticAction(StatementList * tryBlock, CatchClause * catchClause, FinallyClause * finallyClause);
Statement * TryStatementSemanticAction(TryStatement * tryStatement);
Expression * UnaryExpressionSemanticAction(Expression * operand, OperatorType opType, bool isPostfix);
VariableDeclaratorList * VariableDeclaratorListSemanticAction(VariableDeclarator * variableDeclarator);
VariableDeclarator * VariableDeclaratorSemanticAction(const char * identifier, Expression * initializer);
WhileStatement * WhileSemanticAction(Expression * cond, Statement * body);
Statement * WhileStatementSemanticAction(WhileStatement * ws);
Expression * MemberExpressionSemanticAction(Expression *base, const char *id);
Expression * SubscriptExpressionSemanticAction(Expression *base, Expression *index);
Expression * ArrayLiteralSemanticAction(ArgumentList *elements);
StatementListItem * FunctionDeclarationStatementListItemSemanticAction(FunctionDeclaration * d);
StatementListItem * LexicalDeclarationStatementListItemSemanticAction(LexicalDeclaration * d);
#endif
