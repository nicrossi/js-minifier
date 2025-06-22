#include "ExpressionGenerator.h"
#include "Generator.h"

/* MODULE INTERNAL STATE */
static Logger * _logger = NULL;

void initializeExpressionGeneratorModule() {
    _logger = createLogger("ExpressionGenerator");
}

void shutdownExpressionGeneratorModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

static void _wprEmitInteger(Expression * expression, const char * operator);
static void _wprEmitString(Expression * expression, const char * operator);
static void _wprEmitIdentifier(Expression * expression, const char * operator);
static void _emitIdentifier(Expression * expression);
static void _emitString(Expression * expression);
static void _emitInteger(Expression * expression);
static void _emitBinary(Expression * expression, const char * operator);
static void _wprEmitCallExpression(Expression * expression, const char * operator);
static void _emitCallExpression(Expression * expression);
static void _emitArguments(const Argument * arg);
static void _wprEmitNew(Expression * expression, const char * operator);
static void _emitNew(Expression * expression);
static void _wprEmitBooleanLiteral(Expression * expression, const char * operator);
static void _emitBooleanLiteral(Expression * expression);
static void _emitUpdateOp(Expression * expression, const char *operator);
static void _wprEmitArrayLiteral(Expression * expression, const char * operator);
static void _emitArrayLiteral(Expression * expression);
static void _wprEmitSubscript(Expression * expression, const char * operator);
static void _emitSubscript(Expression * expression);

typedef void (* ExpressionGenFn) (Expression * expression, const char * operator);
typedef struct {
    ExpressionGenFn fn;
    const char * operator;
} ExpressionGenEntry;


static const ExpressionGenEntry _expressionGenTable[] = {
        // [ExpressionType] = handlerFunction
        [ARRAY_LITERAL_EXPRESSION] = { _wprEmitArrayLiteral, NULL },
        [ASSIGNMENT] = {_emitBinary, "=" },
        [BOOLEAN_LITERAL_EXPRESSION] = { _wprEmitBooleanLiteral, NULL },
        [CALL_EXPRESSION] = { _wprEmitCallExpression, NULL },
        [DIVISION_EXPRESSION] = { _emitBinary, "/" },
        [EQUALITY_EXPRESSION] = {_emitBinary, "==" },
        [EXPONENTIATION_EXPRESSION] = { _emitBinary, "**" },
        [GREATER_EXPRESSION] = {_emitBinary, ">" },
        [GREAT_EQUAL_EXPRESSION] = {_emitBinary, ">=" },
        [IDENTIFIER] = {_wprEmitIdentifier, NULL },
        [INEQUALITY_EXPRESSION] = {_emitBinary, "!=" },
        [INTEGER_EXPRESSION] = {_wprEmitInteger, NULL },
        [LESS_EXPRESSION] = {_emitBinary, "<" },
        [LESS_EQUAL_EXPRESSION] = { _emitBinary, "<=" },
        [LOGICAL_OR_EXPRESSION] = { _emitBinary, "||" },
        [LOGICAL_AND_EXPRESSION] = { _emitBinary, "&&" },
        [MEMBER_EXPRESSION] = { _emitBinary, "." },
        [MULTIPLICATION_EXPRESSION] = { _emitBinary, "*" },
        [NEW_EXPRESSION] = { _wprEmitNew, NULL },
        [POSTFIX_INCREMENT_EXPR] = { _emitUpdateOp, "++" },
        [POSTFIX_DECREMENT_EXPR] = { _emitUpdateOp, "--" },
        [POWER_EXPRESSION] = { _emitBinary, "^" },
        [PREFIX_INCREMENT_EXPR] = { _emitUpdateOp, "++" },
        [PREFIX_DECREMENT_EXPR] = { _emitUpdateOp, "--" },
        [REMAINDER_EXPRESSION] = { _emitBinary, "%" },
        [STRICT_EQUALITY_EXPRESSION] = {_emitBinary, "===" },
        [STRICT_INEQUALITY_EXPRESSION] = {_emitBinary, "!==" },
        [STRING_LITERAL_EXPRESSION] = {_wprEmitString, NULL },
        [SUB_EXPRESSION] = { _emitBinary, "-" },
        [SUBSCRIPT_EXPRESSION] = {  _wprEmitSubscript, NULL },
        [SUM_EXPRESSION] = { _emitBinary, "+" },
};

void genExpression(Expression * expression) {
    if (expression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL expression.");
        return;
    }

    logDebugging(_logger, "Generating output for expression of type: %d", expression->type);
    (expression->type < ARRAY_LEN(_expressionGenTable) && _expressionGenTable[expression->type].fn != NULL)
    ? _expressionGenTable[expression->type].fn(expression, _expressionGenTable[expression->type].operator)
    : logError(_logger, "Unknown expression type: %d", expression->type);
}

static void _emitBinary(Expression * expression, const char * operator) {
    if (expression == NULL ||(expression->binaryExpression.leftExpression == NULL
        && expression->binaryExpression.rightExpression == NULL)) {
        logError(_logger, "Attempt to generate output for a NULL assignment expression.");
        return;
    }

    logDebugging(_logger, "Generating output for assignment expression.");
    genExpression(expression->binaryExpression.leftExpression);
    EMIT(operator);
    genExpression(expression->binaryExpression.rightExpression);
}

static void _wprEmitIdentifier(Expression * expression, const char * operator) { _emitIdentifier(expression); }

static void _emitIdentifier(Expression * expression) {
    logDebugging(_logger, "Generating output for identifier expression with name: \"%s\"", expression->identifierName);
    EMIT("%s", expression->identifierName);
}

static void _wprEmitInteger(Expression * expression, const char * operator) { _emitInteger(expression); }

static void _emitInteger(Expression * expression) {
    logDebugging(_logger, "Generating output for integer expression with value: %d", expression->value);
    EMIT("%d", expression->value);
}

static void _wprEmitString(Expression * expression, const char * operator) { _emitString(expression); }

static void _emitString(Expression * expression) {
    logDebugging(_logger, "Generating output for string expression with value: \"%s\"", expression->string);
    EMIT("\"%s\"", expression->string);
}

static void _wprEmitCallExpression(Expression * expression, const char * operator) { _emitCallExpression(expression); }

static void _emitArguments(const Argument * arg) {
    if (arg == NULL) return;
    genExpression(arg->expression);
    if (arg->next != NULL) {
        EMIT(",");
        _emitArguments(arg->next);
    }
}

static void _emitCallExpression(Expression * expression) {
    if (expression == NULL || expression->callExpression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL call expression.");
        return;
    }

    logDebugging(_logger, "Generating output for call expression.");
    genExpression(expression->callExpression->callee);
    EMIT("(");
    if (expression->callExpression->argumentList != NULL) {
        _emitArguments(expression->callExpression->argumentList->head);
    }
    EMIT(")");
}

static void _emitNew(Expression * expression) {
    if (expression == NULL || expression->callExpression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL new expression.");
        return;
    }

    logDebugging(_logger, "Generating output for new expression.");
    EMIT("new ");
    genExpression(expression->callExpression->callee);
    if (expression->callExpression->argumentList != NULL) {
        EMIT("(");
        _emitArguments(expression->callExpression->argumentList->head);
        EMIT(")");
    }
}

static void _wprEmitNew(Expression * expression, const char * operator) {
    _emitNew(expression);
}

static void _wprEmitBooleanLiteral(Expression * expression, const char * operator) {
    _emitBooleanLiteral(expression);
}

static void _emitBooleanLiteral(Expression * expression) {
    if (expression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL boolean literal expression.");
        return;
    }
    EMIT("%s", expression->value ? "!0" : "!1");
}

static void _emitUpdateOp(Expression * expression, const char * operator) {
    if (!expression || !expression->updateOp) {
        logError(_logger, "Attempt to generate output for a NULL update expression.");
        return;
    }

    if (operator == NULL) {
        logError(_logger, "Operator string is NULL in _emitUpdateOp.");
        return;
    }

    if (expression->updateOp->isPostfix) {
        genExpression(expression->updateOp->operand);
        EMIT("%s", operator);
    } else {
        EMIT("%s", operator);
        genExpression(expression->updateOp->operand);
    }
}

static void _emitArrayLiteral(Expression * expression) {
    if (expression == NULL || expression->callExpression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL array literal expression.");
        return;
    }

    logDebugging(_logger, "Generating output for array literal expression.");
    EMIT("[");
    if (expression->callExpression->argumentList != NULL) {
        _emitArguments(expression->callExpression->argumentList->head);
    }
    EMIT("]");
}

static void _wprEmitArrayLiteral(Expression * expression, const char * operator) {
    _emitArrayLiteral(expression);
}

static void _wprEmitSubscript(Expression * expression, const char * operator) {
    _emitSubscript(expression);
}

static void _emitSubscript(Expression * expression) {
    if (expression == NULL || expression->binaryExpression.leftExpression == NULL
        || expression->binaryExpression.rightExpression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL subscript expression.");
        return;
    }

    logDebugging(_logger, "Generating output for subscript expression.");
    genExpression(expression->binaryExpression.leftExpression);
    EMIT("[");
    genExpression(expression->binaryExpression.rightExpression);
    EMIT("]");
}