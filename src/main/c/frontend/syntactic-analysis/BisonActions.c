#include <assert.h>
#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */
VariableDeclaratorList * AppendVariableDeclaratorListSemanticAction(VariableDeclaratorList * list, VariableDeclarator * variableDeclarator) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    assert(list->head != NULL);
    variableDeclarator->next = NULL;
    list->tail->next = variableDeclarator;
    list->tail = variableDeclarator;
    return list;
}

Expression * AssignmentExpressionSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return expression;
}

Expression * ChainedAssignmentSemanticAction(Expression * left, Expression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = ASSIGNMENT;
    expression->binaryExpression.leftExpression = left;
    expression->binaryExpression.rightExpression = right;
    return expression;
}

Expression * CommaExpressionSemanticAction(Expression * expression, Expression * assignmentExpression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    // Binary expression for the comma-separated expressions
    Expression * newExpression = ecalloc(1, sizeof(Expression));
    newExpression->type = ASSIGNMENT; // TODO: Change to appropriate type
    newExpression->binaryExpression.leftExpression = expression;
    newExpression->binaryExpression.rightExpression = assignmentExpression;
    return newExpression;
}

StatementListItem * DeclarationStatementListItemSemanticAction(Declaration * declaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementListItem * item = ecalloc(1, sizeof(StatementListItem));
    item->declaration = declaration;
    item->next = NULL;
    return item;
}

StatementList * EmptyStatementListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementList * statementList = ecalloc(1, sizeof(StatementList));
    statementList->head = statementList->tail = NULL;
    return statementList;
}

Expression * IntegerExpressionSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->type = ASSIGNMENT;
    expression->value = value;
	return expression;
}

LexicalConst * LexicalConstSemanticAction(VariableDeclaratorList * variableDeclaratorList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    LexicalConst * lexicalConst = ecalloc(1, sizeof(LexicalConst));
    lexicalConst->declaratorList = variableDeclaratorList;
    return lexicalConst;
}

Declaration * LexicalConstDeclarationSemanticAction(LexicalConst * lexicalConst) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Declaration * declaration = ecalloc(1, sizeof(Declaration));
    declaration->lexicalConst = lexicalConst;
    return declaration;
}

Program * StatementListProgramSemanticAction(CompilerState * compilerState, StatementList * statementList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Program * program = ecalloc(1, sizeof(Program));
    program->statementList = statementList;
    compilerState->abstractSyntaxtTree = program;
    if (0 < flexCurrentContext()) {
        logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
        compilerState->succeed = false;
    }
    else {
        compilerState->succeed = true;
    }
    return program;
}

StatementList * StatementListSemanticAction(StatementList * statementList, StatementListItem * statementListItem) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (statementList->head == NULL) {
        statementList->head = statementListItem;
        statementList->tail = statementListItem;
    } else {
        statementList->tail->next = statementListItem;
        statementList->tail = statementListItem;
    }
    return statementList;
}

StatementListItem * StatementStatementListItemSemanticAction(Statement * statement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementListItem * item = ecalloc(1, sizeof(StatementListItem));
    item->statement = statement;
    item->next = NULL;
    return item;
}

VariableDeclaratorList * VariableDeclaratorListSemanticAction(VariableDeclarator * variableDeclarator) {
    _logSyntacticAnalyzerAction(__FUNCTION__ );
    VariableDeclaratorList * variableDeclaratorList = ecalloc(1, sizeof(VariableDeclaratorList));
    variableDeclaratorList->head = variableDeclaratorList->tail = variableDeclarator;
    return variableDeclaratorList;
}

VariableDeclarator * VariableDeclaratorSemanticAction(const char * identifier, Expression * initializer) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    VariableDeclarator * declarator = ecalloc(1, sizeof(VariableDeclarator));
    declarator->identifier = strdup(identifier);
    declarator->initializer = initializer;
    declarator->next = NULL;
    free((char *) identifier);
    return declarator;
}