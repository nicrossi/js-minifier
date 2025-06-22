#include "SemanticAnalyzer.h"

static Logger * _logger = NULL;
static SymbolTable * _symTable = NULL;
static bool _ok = true;        /* pessimistic-fail flag */

#define ERR(fmt, ...)  do{ logError   (_logger, fmt, ##__VA_ARGS__); _ok = false; }while(0)

typedef void (* StatementHandler)(const Statement * stmt);

typedef bool (* ExpressionHandler)(Expression * expression);

static void _checkProgram(const Program * program);

static void _checkStatementList(const StatementList * statementList);

static void _checkStatementListItem(const StatementListItem * it);

static void _checkStatementListRec(const StatementListItem * it);

static void _checkStatement(const Statement * stmt);

static void _checkLexicalDeclaration(const LexicalDeclaration * ld);

static void _checkFunctionDeclaration(const FunctionDeclaration * fd);

static bool _checkExpression(Expression * e);

static void _handleBlock(const Statement * stmt);

static void _handleIf(const Statement * stmt);

static void _handleFor(const Statement * stmt);

static void _handleWhile(const Statement * stmt);

static void _handleTry(const Statement * stmt);

static void _handleExpr(const Statement * stmt);

static void _handleNoop(const Statement * stmt);

static bool _handleBinaryExpr(Expression * expression);

static bool _handleIdentifierExpr(Expression * e);

static bool _handleLiteralExpr(Expression * e);

static bool _handleCallExpr(Expression * e);

static bool _handleUpdateExpr(Expression * e);

static bool _handleNoopExpr(Expression * e);

static void _checkLexicalDeclaration(const LexicalDeclaration * ld);

static void _checkFunctionDeclaration(const FunctionDeclaration * fd);

static bool _checkExpression(Expression * e);

static const SymbolInfo * _lookupSymbol(const char * id);

static void _rejectConstWrite(const Expression * expression);

static bool _newIntegerLiteral(Expression * expression, int value);

static bool _newBooleanLiteral(Expression * expression, bool value);

static bool _newStringLiteral(Expression * expression, const char * left, const char * right);

static bool _addSymbol(const char * id, SymKind kind);

static bool _addParam(const char * id);

static void _replaceWithLiteral(Expression * expression, ExpressionType newType, int intVal, const char * strVal);

static StatementHandler _statementHandlers[] = {
        [BLOCK_STATEMENT]         = _handleBlock,
        [IF_STATEMENT]            = _handleIf,
        [FOR_STATEMENT]           = _handleFor,
        [WHILE_STATEMENT]         = _handleWhile,
        [DO_WHILE_STATEMENT]      = _handleWhile,
        [TRY_STATEMENT]           = _handleTry,
        [EXPRESSION_STATEMENT]    = _handleExpr,
        [RETURN_STATEMENT]        = _handleExpr,
        [THROW_STATEMENT]         = _handleExpr,
};

static ExpressionHandler _expressionHandlers[] = {
        // Binary expressions
        [ASSIGNMENT]                   = _handleBinaryExpr,
        [SUM_EXPRESSION]               = _handleBinaryExpr,
        [SUB_EXPRESSION]               = _handleBinaryExpr,
        [MULTIPLICATION_EXPRESSION]    = _handleBinaryExpr,
        [DIVISION_EXPRESSION]          = _handleBinaryExpr,
        [REMAINDER_EXPRESSION]         = _handleBinaryExpr,
        [EXPONENTIATION_EXPRESSION]    = _handleBinaryExpr,
        [LESS_EXPRESSION]              = _handleBinaryExpr,
        [LESS_EQUAL_EXPRESSION]        = _handleBinaryExpr,
        [GREATER_EXPRESSION]           = _handleBinaryExpr,
        [GREAT_EQUAL_EXPRESSION]       = _handleBinaryExpr,
        [EQUALITY_EXPRESSION]          = _handleBinaryExpr,
        [INEQUALITY_EXPRESSION]        = _handleBinaryExpr,
        [STRICT_EQUALITY_EXPRESSION]   = _handleBinaryExpr,
        [STRICT_INEQUALITY_EXPRESSION] = _handleBinaryExpr,
        [LOGICAL_AND_EXPRESSION]       = _handleBinaryExpr,
        [LOGICAL_OR_EXPRESSION]        = _handleBinaryExpr,
        // Literal expressions
        [IDENTIFIER]                   = _handleIdentifierExpr,
        [INTEGER_EXPRESSION]           = _handleLiteralExpr,
        [STRING_LITERAL_EXPRESSION]    = _handleLiteralExpr,
        [BOOLEAN_LITERAL_EXPRESSION]   = _handleLiteralExpr,

        [CALL_EXPRESSION]              = _handleCallExpr,
        [POSTFIX_INCREMENT_EXPR]       = _handleUpdateExpr,
        [PREFIX_INCREMENT_EXPR]        = _handleUpdateExpr,
        [POSTFIX_DECREMENT_EXPR]       = _handleUpdateExpr,
        [PREFIX_DECREMENT_EXPR]        = _handleUpdateExpr,
};

/*  Public functions */

void initializeSemanticAnalyzerModule(void) {
    _logger = createLogger("SemanticAnalyzer");
    _symTable = stCreate();
}

void shutdownSemanticAnalyzerModule(void) {
    if (_symTable) {
        stDestroy(_symTable);
        _symTable = NULL;
    }
    if (_logger) {
        destroyLogger(_logger);
        _logger = NULL;
    }
}

bool validateProgram(const Program * program) {
    assert(_logger && _symTable && "SemanticAnalyzer not initialised");
    _ok = true;

    stEnterScope(_symTable); // global
    _checkProgram(program);
    stExitScope(_symTable);

    return _ok;
}

const SymbolTable * getSymbolTable(void) {
    assert(_symTable && "SemanticAnalyzer not initialised");
    return _symTable;
}

/* Private functions */

static void _checkProgram(const Program * p) {
    if (!p) return;

    _checkStatementList(p->statementList);
}

/*  Statement-list traversal */
static void _checkStatementListRec(const StatementListItem * it) {
    if (!it) return;

    _checkStatementListItem(it);
    _checkStatementListRec(it->next);
}

static void _checkStatementList(const StatementList * statementList) {
    _checkStatementListRec(statementList ? statementList->head : NULL);
}

static void _checkStatementListItem(const StatementListItem * it) {
    if (it->type == DECLARATION_STATEMENT) {
        switch (it->declaration->type) {
            case FUNCTION:
                _checkFunctionDeclaration(it->declaration->functionDeclaration);
                break;
            case LEXICAL:
                _checkLexicalDeclaration(it->declaration->lexicalDeclaration);
                break;
            default:
                _handleNoop(NULL);
        }
    } else {
        _checkStatement(it->statement);
    }
}

/*  Single statement */
static void _checkStatement(const Statement * stmt) {
    if (!stmt) return;
    if (stmt->type < 0 || stmt->type >= (int) (sizeof(_statementHandlers) / sizeof(_statementHandlers[0])) || !_statementHandlers[stmt->type]) {
        _handleNoop(stmt);
        return;
    }
    _statementHandlers[stmt->type](stmt);
}

static void _handleNoop(const Statement * stmt) {
    (void) stmt;
}

static void _handleBlock(const Statement * stmt) {
    stEnterScope(_symTable);
    _checkStatementList(stmt->block);
    stExitScope(_symTable);
}

static void _handleIf(const Statement * stmt) {
    _checkExpression(stmt->ifStatement->condition);
    _checkStatement(stmt->ifStatement->thenStatement);
    _checkStatement(stmt->ifStatement->elseStatement);
}

static void _handleFor(const Statement * stmt) {
    stEnterScope(_symTable);
    if (stmt->forStatement->initializer) {
        switch (stmt->forStatement->initializer->type) {
            case LEXICAL_DECLARATION_FOR_INIT:
                _checkLexicalDeclaration(stmt->forStatement->initializer->lexicalDeclaration);
                break;
            case EXPRESSION_FOR_INIT:
                _checkExpression(stmt->forStatement->initializer->expression);
                break;
            default:
                break;
        }
    }
    _checkExpression(stmt->forStatement->condition);
    _checkExpression(stmt->forStatement->increment);
    _checkStatement(stmt->forStatement->body);
    stExitScope(_symTable);
}

static void _handleWhile(const Statement * stmt) {
    _checkExpression(stmt->whileStatement->condition);
    _checkStatement(stmt->whileStatement->body);
}

static void _handleTry(const Statement * stmt) {
    _checkStatementList(stmt->tryStatement->tryBlock);
    if (stmt->tryStatement->catchClause) {
        stEnterScope(_symTable);
        if (stmt->tryStatement->catchClause->identifier) {
            _addSymbol(stmt->tryStatement->catchClause->identifier, SYM_LET);
        }
        _checkStatementList(stmt->tryStatement->catchClause->block);
        stExitScope(_symTable);
    }
    if (stmt->tryStatement->finallyClause) {
        _checkStatementList(stmt->tryStatement->finallyClause->block);
    }
}

static void _handleExpr(const Statement * stmt) {
    _checkExpression(stmt->expression);
}

/*  Declarations */
static void _checkLexicalDeclaration(const LexicalDeclaration * ld) {
    if (!ld) return;

    SymKind kind = (ld->type == CONST_DECLARATION) ? SYM_CONST : SYM_LET;

    for (VariableDeclarator * vd = ld->declaratorList->head; vd; vd = vd->next) {
        bool ok = _addSymbol(vd->identifier, kind);

        if (kind == SYM_CONST && vd->initializer == NULL)
            ERR("const '%s' requires an initializer", vd->identifier);

        if (ok && vd->initializer) {
            _checkExpression(vd->initializer);
        }
    }
}

static void _checkFunctionDeclaration(const FunctionDeclaration * fd) {
    /* function name is hoisted in its enclosing scope           */
    if (!_addSymbol(fd->identifier, SYM_FUNC))
        ERR("duplicate declaration of function '%s'", fd->identifier);

    /* new local scope for parameters + body                      */
    stEnterScope(_symTable);

    for (VariableDeclarator * p = fd->parameterList->head; p; p = p->next) {
        _addParam(p->identifier);
    }

    _checkStatementList(fd->body);
    stExitScope(_symTable);
}

/* returns true if expression is a literal after this call */
static bool _checkExpression(Expression * e) {
    if (!e) return false;
    if (e->type < 0 || e->type >= ARRAY_LEN(_expressionHandlers) || !_expressionHandlers[e->type]) {
        return _handleNoopExpr(e);
    }
    return _expressionHandlers[e->type](e);
}

static bool _handleBinaryExpr(Expression * expression) {
    if (expression->type == ASSIGNMENT) {
        _rejectConstWrite(expression->binaryExpression.leftExpression);
    }

    bool lLit = _checkExpression(expression->binaryExpression.leftExpression);
    bool rLit = _checkExpression(expression->binaryExpression.rightExpression);
    if (!lLit || !rLit) {
        return false;
    }

    Expression * L = expression->binaryExpression.leftExpression;
    Expression * R = expression->binaryExpression.rightExpression;
    int lv = L->value;
    int rv = R->value;
    int lt = L->type;
    int rt = R->type;

    switch (expression->type) {
        case SUM_EXPRESSION:
            if (lt == STRING_LITERAL_EXPRESSION && rt == STRING_LITERAL_EXPRESSION) {
                // fold  "foo" + "bar"  →  "foobar"
                return _newStringLiteral(expression, L->string, R->string);
            }
            // fall back to numeric addition when both sides are integers
            if (lt == INTEGER_EXPRESSION && rt == INTEGER_EXPRESSION) {
                return _newIntegerLiteral(expression, lv + rv);
            }
            return false;

        case SUB_EXPRESSION:
            return _newIntegerLiteral(expression, lv - rv);
        case MULTIPLICATION_EXPRESSION:
            return _newIntegerLiteral(expression, lv * rv);
        case LOGICAL_AND_EXPRESSION:
            return _newBooleanLiteral(expression, lv && rv);
        case LOGICAL_OR_EXPRESSION:
            return _newBooleanLiteral(expression, lv || rv);

        case DIVISION_EXPRESSION:
        case REMAINDER_EXPRESSION:
            if (rv == 0) {
                ERR("division by zero");
                return false;
            }
            return (expression->type == DIVISION_EXPRESSION)
                   ? _newIntegerLiteral(expression, lv / rv)
                   : _newIntegerLiteral(expression, lv % rv);

        case EXPONENTIATION_EXPRESSION:
            return _newIntegerLiteral(expression, (int) pow(lv, rv));
        case LESS_EXPRESSION:
            return _newBooleanLiteral(expression, lv < rv);
        case LESS_EQUAL_EXPRESSION:
            return _newBooleanLiteral(expression, lv <= rv);
        case GREATER_EXPRESSION:
            return _newBooleanLiteral(expression, lv > rv);
        case GREAT_EQUAL_EXPRESSION:
            return _newBooleanLiteral(expression, lv >= rv);
        case EQUALITY_EXPRESSION:
            return _newBooleanLiteral(expression, lv == rv);
        case INEQUALITY_EXPRESSION:
            return _newBooleanLiteral(expression, lv != rv);

        case STRICT_EQUALITY_EXPRESSION:
            return _newBooleanLiteral(expression,
                                      lv == rv &&
                                      lt == INTEGER_EXPRESSION && rt == INTEGER_EXPRESSION);

        case STRICT_INEQUALITY_EXPRESSION:
            return _newBooleanLiteral(expression,
                                      lv != rv ||
                                      lt != INTEGER_EXPRESSION || rt != INTEGER_EXPRESSION);

        default:
            break;
    }
    return false;
}


static bool _newStringLiteral(Expression * expression, const char * left, const char * right) {
    if (left && right) {
        char * newStr = calloc(1, strlen(left) + strlen(right) + 1);
        if (!newStr) {
            ERR("out of memory while concatenating strings");
            return false;
        }
        strcpy(newStr, left);
        strcat(newStr, right);
        _replaceWithLiteral(expression, STRING_LITERAL_EXPRESSION, 0, newStr);
        free(newStr);
    } else {
        _replaceWithLiteral(expression, STRING_LITERAL_EXPRESSION, 0, left ? left : right);
    }
    return true;
}

static void _replaceWithLiteral(Expression * expression, ExpressionType newType,
                                int intVal, const char * strVal) {
    assert(expression && "Expression cannot be NULL");

    if (expression->type >= ASSIGNMENT && expression->type <= SUM_EXPRESSION) {
        releaseExpression(expression->binaryExpression.leftExpression);
        releaseExpression(expression->binaryExpression.rightExpression);
    } else if (expression->type == CALL_EXPRESSION) {
        releaseCallExpression(expression->callExpression);
    }

    expression->type = newType;
    if (newType == INTEGER_EXPRESSION || newType == BOOLEAN_LITERAL_EXPRESSION) {
        expression->value = intVal;
    } else if (newType == STRING_LITERAL_EXPRESSION) {
        expression->string = strdup(strVal ? strVal : "");
    }
}


static bool _handleIdentifierExpr(Expression * e) {
    if (!stLookup(_symTable, e->identifierName))
        ERR("use of undeclared identifier '%s'", e->identifierName);
    return false;
}

static bool _handleLiteralExpr(Expression * e) {
    return true; // Always valid, nothing to do
}

static bool _handleCallExpr(Expression * e) {
    _checkExpression(e->callExpression->callee);
    if (e->callExpression->argumentList) {
        for (Argument * a = e->callExpression->argumentList->head; a; a = a->next) {
            _checkExpression(a->expression);
        }
    }
    return false;
}

static bool _handleUpdateExpr(Expression * e) {
    _rejectConstWrite(e->updateOp->operand);
    _checkExpression(e->updateOp->operand);
    return false;
}

static bool _handleNoopExpr(Expression * e) {
    return false; // No action
}

/* Try to enter a new symbol in the current scope. */
static bool _addSymbol(const char * id, SymKind kind) {
    if (!stInsert(_symTable, id, kind)) {
        ERR("duplicate declaration of '%s' in the same scope", id);
        return false;
    }
    return true;
}

static bool _addParam(const char * id) {
    if (!stInsert(_symTable, id, SYM_LET)) {
        ERR("duplicate parameter name '%s'", id);
        return false;
    }
    return true;
}

/* Look up the symbol for a bare-identifier expression.
 * Returns NULL if the expression is not an identifier or the identifier is
 * unknown in the current environment.
 * */
static const SymbolInfo * _lookupIdentifierExpr(const Expression * expression) {
    return (expression && expression->type == IDENTIFIER)
           ? stLookup(_symTable, expression->identifierName)
           : NULL;
}

/* Fail if the l-value is a const. */
static void _rejectConstWrite(const Expression * expression) {
    const SymbolInfo * info = _lookupIdentifierExpr(expression);
    if (info && info->kind == SYM_CONST) {
        ERR("cannot modify '%s' as it declared as constant.", expression->identifierName);
    }
}

static bool _newIntegerLiteral(Expression * expression, int value) {
    _replaceWithLiteral(expression, INTEGER_EXPRESSION, value, NULL);
    return true;
}

static bool _newBooleanLiteral(Expression * expression, bool value) {
    _replaceWithLiteral(expression, BOOLEAN_LITERAL_EXPRESSION, value, NULL);
    return true;
}