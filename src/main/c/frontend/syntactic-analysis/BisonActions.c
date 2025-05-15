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
void checkInitializer(VariableDeclaratorList * list);

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

Expression * AssignmentExpressionSemanticAction(VariableDeclarator * variableDeclarator) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = ASSIGNMENT;
    expression->binaryExpression.leftExpression = ecalloc(1, sizeof(Expression));
    expression->binaryExpression.leftExpression->type = IDENTIFIER;
    expression->binaryExpression.leftExpression->identifierName = strdup(variableDeclarator->identifier);
    expression->binaryExpression.rightExpression = variableDeclarator->initializer;
    free(variableDeclarator->identifier);
    free(variableDeclarator);
    variableDeclarator = NULL;
    return expression;
}

StatementList * BlockSemanticAction(StatementList * statementList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return statementList;
}

Statement * BlockStatementSemanticAction(StatementList * statementList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = BLOCK_STATEMENT;
    statement->block = statementList;
    return statement;
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
    item->type = DECLARATION_STATEMENT;
    item->next = NULL;
    return item;
}

StatementList * EmptyBlockSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementList * statementList = ecalloc(1, sizeof(StatementList));
    statementList->head = statementList->tail = NULL;
    return statementList;
}

StatementList * EmptyStatementListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementList * statementList = ecalloc(1, sizeof(StatementList));
    statementList->head = statementList->tail = NULL;
    return statementList;
}

Expression * EqualityExpressionSemanticAction(Expression * left, Expression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = BOOLEAN_EXPRESSION;
    expression->binaryExpression.leftExpression = left;
    expression->binaryExpression.rightExpression = right;
    return expression;
}

Statement * ExpressionStatementSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = EXPRESSION_STATEMENT;
    statement->expression = expression;
    return statement;
}

Expression * IdentifierExpressionSemanticAction(const char * identifier) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = IDENTIFIER;
    expression->identifierName = strdup(identifier);
    free((char *) identifier);
    return expression;
}

IfStatement * IfSemanticAction(Expression * condition, Statement * thenStatement, Statement * elseStatement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    IfStatement * ifStatement = ecalloc(1, sizeof(IfStatement));
    ifStatement->condition = condition;
    ifStatement->thenStatement = thenStatement;
    ifStatement->elseStatement = elseStatement;
    return ifStatement;
}

Statement * IfStatementSemanticAction(IfStatement * ifStatement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = IF_STATEMENT;
    statement->ifStatement = ifStatement;
    return statement;
}

Expression * IntegerExpressionSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->type = INTEGER_EXPRESSION;
    expression->value = value;
	return expression;
}

LexicalDeclaration * CreateLexicalDeclarationSemanticAction(LexicalDeclarationType type, VariableDeclaratorList * variableDeclaratorList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    if (type == CONST_DECLARATION) {
        checkInitializer(variableDeclaratorList);
    }
    LexicalDeclaration * lexicalDeclaration = ecalloc(1, sizeof(LexicalDeclaration));
    lexicalDeclaration->type = type;
    lexicalDeclaration->declaratorList = variableDeclaratorList;
    return lexicalDeclaration;
}

Declaration * LexicalDeclarationSemanticAction(LexicalDeclaration * lexicalDeclaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Declaration * declaration = ecalloc(1, sizeof(Declaration));
    declaration->lexicalDeclaration = lexicalDeclaration;
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
    assert(statement != NULL);
    StatementListItem * item = ecalloc(1, sizeof(StatementListItem));
    item->statement = statement;
    item->type = statement->type;
    item->next = NULL;
    return item;
}

Expression * StringExpressionSemanticAction(const char * s) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = STRING_LITERAL_EXPRESSION;
    expression->string = strdup(s);
    free((char *) s);
    return expression;
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

void checkInitializer(VariableDeclaratorList * list) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    for (VariableDeclarator  * vd = list->head; vd != NULL; vd = vd->next) {
        if (vd->initializer == NULL) {
            logError(_logger, "Variable \"%s\" is declared without an initializer.", vd->identifier);
            exit(EXIT_FAILURE);
        }
    }
}

