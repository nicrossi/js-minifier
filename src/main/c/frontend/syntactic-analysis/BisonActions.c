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

static CatchClause * _newCatchClause(const char * id, StatementList * blk) {
    CatchClause * c = ecalloc(1, sizeof(CatchClause));
    c->identifier = id ? strdup(id) : NULL;
    c->block = blk;
    if (id) {
        free((char*)id);
    }
    return c;
}

static FinallyClause * _newFinallyClause(StatementList * blk) {
    FinallyClause * f = ecalloc(1, sizeof(FinallyClause));
    f->block = blk;
    return f;
}

static TryStatement * _newTryStatement(StatementList * tryBlk, CatchClause * catchCl,
                                       FinallyClause * finCl) {
    TryStatement * t = ecalloc(1, sizeof(TryStatement));
    t->tryBlock = tryBlk;
    t->catchClause = catchCl;
    t->finallyClause = finCl;
    return t;
}

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

ArgumentList * AppendArgumentListSemanticAction(ArgumentList * argumentList, Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    assert(argumentList != NULL);
    Argument * argument = ecalloc(1, sizeof(Argument));
    argument->expression = expression;
    argument->next = NULL;
    argumentList->tail->next = argument;
    argumentList->tail = argument;
    return argumentList;
}

VariableDeclaratorList * AppendParameterListSemanticAction(VariableDeclaratorList * list,
                                                           VariableDeclarator * parameter) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    assert(list->head != NULL);
    list->tail->next = parameter;
    list->tail = parameter;
    return list;
}

ArgumentList * ArgumentListSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArgumentList * argumentList = ecalloc(1, sizeof(ArgumentList));
    Argument * argument = ecalloc(1, sizeof(Argument));
    argument->expression = expression;
    argument->next = NULL;
    argumentList->head = argumentList->tail =argument;
    return argumentList;
}

Expression * AssignmentExpressionSemanticAction(Expression * left, Expression * right) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = ASSIGNMENT;
    expression->binaryExpression.leftExpression = left;
    expression->binaryExpression.rightExpression = right;
    return expression;
}

Expression * BooleanExpressionSemanticAction(bool v) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression *e = ecalloc(1, sizeof(Expression));
    e->type = BOOLEAN_LITERAL_EXPRESSION;
    e->value = v;
    return e;
}


StatementList * BlockSemanticAction(StatementList * statementList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return statementList;
}

Statement * BreakStatementSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = BREAK_STATEMENT;
    return statement;
}

Statement * BlockStatementSemanticAction(StatementList * statementList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = BLOCK_STATEMENT;
    statement->block = statementList;
    return statement;
}

Expression * CallExpressionSemanticAction(Expression * callee, ArgumentList * argumentList) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = CALL_EXPRESSION;
    CallExpression * call = ecalloc(1, sizeof(CallExpression));
    call->callee = callee;
    call->argumentList = argumentList;
    expression->callExpression = call;
    return expression;
}

CatchClause * CatchClauseSemanticAction(const char * identifier, StatementList * block) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return _newCatchClause(identifier, block);
}

CatchClause * CatchClauseNoParamSemanticAction(StatementList * catchBlock) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return _newCatchClause(NULL, catchBlock);
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

Statement * ContinueStatementSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = CONTINUE_STATEMENT;
    return statement;
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

StatementListItem * LexicalDeclarationStatementListItemSemanticAction(LexicalDeclaration * d) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementListItem * item = ecalloc(1, sizeof(StatementListItem));
    Declaration * declaration = ecalloc(1, sizeof(Declaration));
    declaration->type = LEXICAL;
    declaration->lexicalDeclaration = d;
    item->declaration = declaration;
    item->type = DECLARATION_STATEMENT;
    item->next = NULL;
    return item;
}

StatementListItem * FunctionDeclarationStatementListItemSemanticAction(FunctionDeclaration * d) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementListItem * item = ecalloc(1, sizeof(StatementListItem));
    Declaration * declaration = ecalloc(1, sizeof(Declaration));
    declaration->type = FUNCTION;
    declaration->functionDeclaration = d;
    item->declaration = declaration;
    item->type = DECLARATION_STATEMENT;
    item->next = NULL;
    return item;
}

WhileStatement * DoWhileSemanticAction(Statement * body, Expression * cond) {
    return WhileSemanticAction(cond, body);
}

Statement * DoWhileStatementSemanticAction(WhileStatement * ws) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = DO_WHILE_STATEMENT;
    statement->whileStatement = ws;
    return statement;
}

ArgumentList * EmptyArgumentListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ArgumentList * argumentList = ecalloc(1, sizeof(ArgumentList));
    argumentList->head = argumentList->tail = NULL;
    return argumentList;
}

StatementList * EmptyBlockSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementList * statementList = ecalloc(1, sizeof(StatementList));
    statementList->head = statementList->tail = NULL;
    return statementList;
}

CaseClause * EmptyCaseClauseSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return NULL;
}

Expression * EmptyExpressionSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__ );
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = EMPTY_EXPRESSION;
    return expression;
}

ForInitializer * EmptyForInitSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ForInitializer * forInitializer = ecalloc(1, sizeof(ForInitializer));
    forInitializer->type = EMPTY_FOR_INIT;
    return forInitializer;
}

VariableDeclaratorList * EmptyParameterListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    VariableDeclaratorList * variableDeclaratorList = ecalloc(1, sizeof(VariableDeclaratorList));
    variableDeclaratorList->head = variableDeclaratorList->tail = NULL;
    return variableDeclaratorList;
}

StatementList * EmptyStatementListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    StatementList * statementList = ecalloc(1, sizeof(StatementList));
    statementList->head = statementList->tail = NULL;
    return statementList;
}

Expression * BinaryExpressionSemanticAction(Expression * left, Expression * right, ExpressionType type) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = type;
    expression->binaryExpression.leftExpression = left;
    expression->binaryExpression.rightExpression = right;
    return expression;
}

ForInitializer * ExpressionForInitSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ForInitializer * forInitializer = ecalloc(1, sizeof(ForInitializer));
    forInitializer->type = EXPRESSION_FOR_INIT;
    forInitializer->expression = expression;
    return forInitializer;
}

Statement * ExpressionStatementSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = EXPRESSION_STATEMENT;
    statement->expression = expression;
    return statement;
}

ForStatement * ForIterationSemanticAction(ForInitializer * initializer, Expression * condition,
                                          Expression * increment, Statement * body) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ForStatement * forStatement = ecalloc(1, sizeof(ForStatement));
    forStatement->initializer = initializer;
    forStatement->condition = condition;
    forStatement->increment = increment;
    forStatement->body = body;
    return forStatement;
}

Statement * ForStatementSemanticAction(ForStatement * forStatement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = FOR_STATEMENT;
    statement->forStatement = forStatement;
    return statement;
}

Declaration * FunctionDeclarationSemanticAction(FunctionDeclaration * functionDeclaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Declaration * declaration = ecalloc(1, sizeof(Declaration));
    declaration->type = FUNCTION;
    declaration->functionDeclaration = functionDeclaration;
    return declaration;
}

FunctionDeclaration * FunctionSemanticAction(const char * identifier, VariableDeclaratorList * parameterList, StatementList * body) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    FunctionDeclaration * functionDeclaration = ecalloc(1, sizeof(FunctionDeclaration));
    functionDeclaration->identifier = strdup(identifier);
    functionDeclaration->parameterList = parameterList;
    functionDeclaration->body = body;
    free((char *) identifier);
    return functionDeclaration;
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

ForInitializer * LexicalDeclarationForInitSemanticAction(LexicalDeclaration * forLexicalDeclaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ForInitializer * forInitializer = ecalloc(1, sizeof(ForInitializer));
    forInitializer->type = LEXICAL_DECLARATION_FOR_INIT;
    forInitializer->lexicalDeclaration = forLexicalDeclaration;
    return forInitializer;
}

Declaration * LexicalDeclarationSemanticAction(LexicalDeclaration * lexicalDeclaration) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Declaration * declaration = ecalloc(1, sizeof(Declaration));
    declaration->type = LEXICAL;
    declaration->lexicalDeclaration = lexicalDeclaration;
    return declaration;
}

Expression * NewExpressionSemanticAction(Expression * exp, ArgumentList * args) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    expression->type = NEW_EXPRESSION;
    // Reusing CallExpression structure for new expression
    CallExpression * call = ecalloc(1, sizeof(CallExpression));
    call->callee = exp;
    call->argumentList = args;
    expression->callExpression = call;
    return expression;
}

Expression * OptionalExpression(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return expression;
}

VariableDeclaratorList * ParameterListSemanticAction(const char * parameter) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    VariableDeclaratorList * paramList = ecalloc(1, sizeof(VariableDeclaratorList));
    VariableDeclarator * item = ecalloc(1, sizeof(VariableDeclarator));
    item->identifier = strdup(parameter);
    item->initializer = NULL;
    item->next = NULL;
    free((char *) parameter);
    paramList->head = paramList->tail = item;
    return paramList;
}

Expression * ParenthesisExpressionSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return expression;
}

Statement * ReturnStatementSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = RETURN_STATEMENT;
    statement->expression = expression;
    return statement;
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

SwitchStatement * SwitchSemanticAction(Expression * discriminant, CaseClause * cases) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    SwitchStatement * switchStatement = ecalloc(1, sizeof(SwitchStatement));
    switchStatement->discriminant = discriminant;
    switchStatement->cases = cases;
    return switchStatement;
}

CaseClause * CaseClauseSemanticAction(Expression * test, StatementList * body) {
    CaseClause * c = ecalloc(1, sizeof(CaseClause));
    c->type = CASE_CLAUSE;
    c->test = test;
    c->body = body;
    return c;
}

CaseClause * DefaultClauseSemanticAction(StatementList * body) {
    CaseClause * c = ecalloc(1, sizeof(CaseClause));
    c->type = DEFAULT_CLAUSE;
    c->test = NULL; // indicates ‘default’
    c->body = body;
    return c;
}

CaseClause * AppendCaseClauseSemanticAction(CaseClause * head, CaseClause * c) {
    if (!head) return c;

    CaseClause * it = head;
    while (it->next) it = it->next;
    it->next = c;
    return head;
}

Statement * SwitchStatementSemanticAction(SwitchStatement * sw) {
    Statement * stmt = ecalloc(1, sizeof(Statement));
    stmt->type = SWITCH_STATEMENT;
    stmt->switchStatement = sw;
    return stmt;
}


FinallyClause * FinallyClauseSemanticAction(StatementList * finallyBlock) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return _newFinallyClause(finallyBlock);
}

Statement * ThrowStatementSemanticAction(Expression * expression) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * s = ecalloc(1, sizeof(Statement));
    s->type = THROW_STATEMENT;
    s->expression = expression;
    return s;
}

TryStatement * TryCatchSemanticAction(StatementList * tryBlock, CatchClause * catchClause) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return _newTryStatement(tryBlock, catchClause, NULL);
}

TryStatement * TryFinallySemanticAction(StatementList * tryBlock, FinallyClause * finallyClause) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return _newTryStatement(tryBlock, NULL, finallyClause);
}

TryStatement * TryCatchFinallySemanticAction(StatementList * tryBlock, CatchClause * catchClause,
                                             FinallyClause * finallyClause) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return _newTryStatement(tryBlock, catchClause, finallyClause);
}

Statement * TryStatementSemanticAction(TryStatement * tryStatement) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1, sizeof(Statement));
    statement->type = TRY_STATEMENT;
    statement->tryStatement = tryStatement;
    return statement;
}

Expression * UnaryExpressionSemanticAction(Expression * operand, OperatorType opType, bool isPostfix) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expression = ecalloc(1, sizeof(Expression));
    UpdateOp * updateOp = ecalloc(1, sizeof(UpdateOp));
    updateOp->operand = operand;
    updateOp->operator = opType;
    updateOp->isPostfix = isPostfix;
    expression->type = isPostfix
        ? (opType == INCREMENT_OP) ? POSTFIX_INCREMENT_EXPR : POSTFIX_DECREMENT_EXPR
        : (opType == INCREMENT_OP) ? PREFIX_INCREMENT_EXPR : PREFIX_DECREMENT_EXPR;
    expression->updateOp = updateOp;
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

WhileStatement * WhileSemanticAction(Expression * cond, Statement * body) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    WhileStatement * ws = ecalloc(1,sizeof(WhileStatement));
    ws->condition = cond;
    ws->body = body;
    return ws;
}

Statement * WhileStatementSemanticAction(WhileStatement * ws) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Statement * statement = ecalloc(1,sizeof(Statement));
    statement->type = WHILE_STATEMENT;
    statement->whileStatement = ws;
    return statement;
}

Expression * MemberExpressionSemanticAction(Expression * base, const char * identifier) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * idExpr = IdentifierExpressionSemanticAction(identifier);
    return BinaryExpressionSemanticAction(base,idExpr, MEMBER_EXPRESSION);
}

Expression * SubscriptExpressionSemanticAction(Expression * base, Expression * index) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    return BinaryExpressionSemanticAction(base,index,SUBSCRIPT_EXPRESSION);
}

Expression * ArrayLiteralSemanticAction(ArgumentList * elements) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Expression * expr = ecalloc(1, sizeof(Expression));
    expr->type = ARRAY_LITERAL_EXPRESSION;
    CallExpression * pseudo = ecalloc(1, sizeof(CallExpression));
    pseudo->callee = NULL;
    pseudo->argumentList = elements;
    expr->callExpression = pseudo;
    return expr;
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
