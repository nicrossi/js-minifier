#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */
void releaseArgument(Argument * argument) { //NOLINT
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (argument == NULL) return;
    releaseExpression(argument->expression);
    free(argument);
    argument = NULL;
}
void releaseArgumentList(ArgumentList * list) { //NOLINT
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (list == NULL) return;
    Argument * current = list->head;
    while (current != NULL) {
        Argument * next = current->next;
        releaseArgument(current);
        current = next;
    }
    free(list);
    list = NULL;
}
void releaseDeclaration(Declaration * declaration) { //NOLINT
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (declaration == NULL) return;

    if (declaration->type == FUNCTION) {
        releaseFunctionDeclaration(declaration->functionDeclaration);
    } else if (declaration->type == LEXICAL) {
        releaseLexicalDeclaration(declaration->lexicalDeclaration);
    }
    free(declaration);
    declaration = NULL;
}

void releaseExpression(Expression * expression) { //NOLINT
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (expression != NULL) {
        switch(expression->type) {
            case ASSIGNMENT: case EQUALITY_EXPRESSION: case INEQUALITY_EXPRESSION:
            case STRICT_EQUALITY_EXPRESSION: case STRICT_INEQUALITY_EXPRESSION:
            case SUB_EXPRESSION: case SUM_EXPRESSION: case POWER_EXPRESSION:
            case MULTIPLICATION_EXPRESSION: case DIVISION_EXPRESSION: case REMAINDER_EXPRESSION:
            case GREAT_EQUAL_EXPRESSION: case LESS_EQUAL_EXPRESSION:
            case GREATER_EXPRESSION: case LESS_EXPRESSION: case EXPONENTIATION_EXPRESSION:
                releaseExpression(expression->binaryExpression.leftExpression);
                releaseExpression(expression->binaryExpression.rightExpression);
                break;
            case IDENTIFIER: free(expression->identifierName); break;
            case STRING_LITERAL_EXPRESSION: free(expression->string); break;
            case POSTFIX_DECREMENT_EXPR: case PREFIX_INCREMENT_EXPR:
            case PREFIX_DECREMENT_EXPR:  case POSTFIX_INCREMENT_EXPR:
                releaseUpdateOp(expression->updateOp);
                break;
            case CALL_EXPRESSION: releaseCallExpression(expression->callExpression); break;
            default:
                logWarning(_logger, "Unknown expression type: %d", expression->type);
        }
        free(expression);
        expression = NULL;
    }
}

void releaseCallExpression(CallExpression * callExpression) { //NOLINT
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (callExpression == NULL) return;
    releaseExpression(callExpression->callee);
    releaseArgumentList(callExpression->argumentList);
    free(callExpression);
    callExpression = NULL;
}

void releaseFunctionDeclaration(FunctionDeclaration * functionDeclaration) { //NOLINT
    if (functionDeclaration == NULL) return;
    free(functionDeclaration->identifier);
    releaseVariableDeclaratorList(functionDeclaration->parameterList);
    releaseStatementList(functionDeclaration->body);
    free(functionDeclaration);
    functionDeclaration = NULL;
}

void releaseForInitializer(ForInitializer * forInitializer) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (forInitializer != NULL) {
        if (forInitializer->type == LEXICAL_DECLARATION_FOR_INIT) {
            releaseLexicalDeclaration(forInitializer->lexicalDeclaration);
        } else if (forInitializer->type == EXPRESSION_FOR_INIT) {
            releaseExpression(forInitializer->expression);
        }
        free(forInitializer);
        forInitializer = NULL;
    }
}

void releaseForStatement(ForStatement * forStatement) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (forStatement != NULL) {
        releaseExpression(forStatement->condition);
        releaseExpression(forStatement->increment);
        releaseStatement(forStatement->body);
        releaseForInitializer(forStatement->initializer);
        free(forStatement);
        forStatement = NULL;
    }
}

void releaseIfStatement(IfStatement * ifStatement) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (ifStatement != NULL) {
        releaseExpression(ifStatement->condition);
        releaseStatement(ifStatement->thenStatement);
        releaseStatement(ifStatement->elseStatement);
        free(ifStatement);
        ifStatement = NULL;
    }
}

void releaseLexicalDeclaration(LexicalDeclaration * lexicalConst) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (lexicalConst != NULL) {
        releaseVariableDeclaratorList(lexicalConst->declaratorList);
        free(lexicalConst);
        lexicalConst = NULL;
    }
}

void releaseStatement(Statement * statement) { //NOLINT
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (statement != NULL) {
        switch(statement->type) {
            case EXPRESSION_STATEMENT: releaseExpression(statement->expression); break;
            case IF_STATEMENT:         releaseIfStatement(statement->ifStatement); break;
            case BLOCK_STATEMENT:      releaseStatementList(statement->block); break;
            case FOR_STATEMENT:        releaseForStatement(statement->forStatement); break;
            case RETURN_STATEMENT:     releaseExpression(statement->expression); break;
            default:
                logWarning(_logger, "Unknown statement type: %d", statement->type);
        }
        free(statement);
    }
}

void releaseStatementList(StatementList * statementList) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (statementList != NULL) {
        StatementListItem * current = statementList->head;
        while (current != NULL) {
            StatementListItem * next = current->next;
            releaseStatementListItem(current);
            current = next;
        }
        free(statementList);
        statementList = NULL;
    }
}

void releaseStatementListItem(StatementListItem * statementListItem) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (statementListItem != NULL) {
        if (statementListItem->type == DECLARATION_STATEMENT) {
            releaseDeclaration(statementListItem->declaration);
        } else if (statementListItem->statement != NULL) {
            releaseStatement(statementListItem->statement);
        }
        free(statementListItem);
        statementListItem = NULL;
    }
}

void releaseString(char * string) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    free(string);
    string = NULL;
}

void releaseUpdateOp(UpdateOp * updateOp) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (updateOp != NULL) {
        releaseExpression(updateOp->operand);
        free(updateOp);
        updateOp = NULL;
    }
}

void releaseVariableDeclarator(VariableDeclarator * variableDeclarator) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (variableDeclarator != NULL) {
        free(variableDeclarator->identifier);
        releaseExpression(variableDeclarator->initializer);
        free(variableDeclarator);
        variableDeclarator = NULL;
    }
}

void releaseVariableDeclaratorList(VariableDeclaratorList * variableDeclaratorList) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (variableDeclaratorList != NULL) {
        VariableDeclarator * current = variableDeclaratorList->head;
        while (current != NULL) {
            VariableDeclarator * next = current->next;
            releaseVariableDeclarator(current);
            current = next;
        }
        free(variableDeclaratorList);
        variableDeclaratorList = NULL;
    }
}

void releaseProgram(Program * program) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (program != NULL) {
        releaseStatementList(program->statementList);
        free(program);
        program = NULL;
    }
}
