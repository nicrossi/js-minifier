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

typedef void (* ExpressionGenFn) (Expression * expression);

static const ExpressionGenFn _expressionGenTable[] = {
        // [ExpressionType] = handlerFunction
//        [ARRAY_LITERAL_EXPRESSION] = _emitArrayLiteral,
        [ASSIGNMENT] = emitAssignment,
//        [BOOLEAN_LITERAL_EXPRESSION] = _emitBooleanLiteral,
//        [CALL_EXPRESSION] = _emit,
//        [DIVISION_EXPRESSION] = _emit,
//        [EQUALITY_EXPRESSION] = _emit,
//        [EMPTY_EXPRESSION] = _emit,
//        [EXPONENTIATION_EXPRESSION] = _emit,
//        [GREATER_EXPRESSION] = _emit,
//        [GREAT_EQUAL_EXPRESSION] = _emit,
        [IDENTIFIER] = emitIdentifier,
//        [INEQUALITY_EXPRESSION] = _emit,
        [INTEGER_EXPRESSION] = emitInteger,
//        [LESS_EXPRESSION] = _emit,
//        [LESS_EQUAL_EXPRESSION] = _emit,
//        [LOGICAL_OR_EXPRESSION] = _emit,
//        [LOGICAL_AND_EXPRESSION] = _emit,
//        [MEMBER_EXPRESSION] = _emit,
//        [MULTIPLICATION_EXPRESSION] = _emit,
//        [NEW_EXPRESSION] = _emit,
//        [POSTFIX_INCREMENT_EXPR] = _emit,
//        [POSTFIX_DECREMENT_EXPR] = _emit,
//        [POWER_EXPRESSION] = _emit,
//        [PREFIX_INCREMENT_EXPR] = _emit,
//        [PREFIX_DECREMENT_EXPR] = _emit,
//        [REMAINDER_EXPRESSION] = _emit,
//        [STRICT_EQUALITY_EXPRESSION] = _emit,
//        [STRICT_INEQUALITY_EXPRESSION] = _emit,
        [STRING_LITERAL_EXPRESSION] = emitString,
//        [SUB_EXPRESSION] = _emit,
//        [SUBSCRIPT_EXPRESSION] = _emit,
//        [SUM_EXPRESSION] = _emit,
};

void genExpression(Expression * expression) {
    if (expression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL expression.");
        return;
    }

    logDebugging(_logger, "Generating output for expression of type: %d", expression->type);
    (expression->type < ARRAY_LEN(_expressionGenTable) && _expressionGenTable[expression->type] != NULL)
    ? _expressionGenTable[expression->type](expression)
    : logError(_logger, "Unknown expression type: %d", expression->type);
}

void emitAssignment(Expression * expression) {
    if (expression == NULL ||(expression->binaryExpression.leftExpression == NULL
        && expression->binaryExpression.rightExpression == NULL)) {
        logError(_logger, "Attempt to generate output for a NULL assignment expression.");
        return;
    }

    logDebugging(_logger, "Generating output for assignment expression.");
    genExpression(expression->binaryExpression.leftExpression);
    EMIT("=");
    genExpression(expression->binaryExpression.rightExpression);
}

void emitIdentifier(Expression * expression) {
    logDebugging(_logger, "Generating output for identifier expression with name: \"%s\"", expression->identifierName);
    EMIT("%s", expression->identifierName);
}

void emitInteger(Expression * expression) {
    logDebugging(_logger, "Generating output for integer expression with value: %d", expression->value);
    EMIT("%d", expression->value);
}

void emitString(Expression * expression) {
    logDebugging(_logger, "Generating output for string expression with value: \"%s\"", expression->string);
    EMIT("\"%s\"", expression->string);
}
