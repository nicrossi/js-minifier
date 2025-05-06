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
void releaseDeclaration(Declaration * declaration) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (declaration != NULL) {
        releaseLexicalConst(declaration->lexicalConst);
        free(declaration);
        declaration = NULL;
    }
}

void releaseExpression(Expression * expression) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (expression != NULL) {
        // TODO: Add specific cleanup logic for different expression types
        free(expression);
        expression = NULL;
    }
}

void releaseLexicalConst(LexicalConst * lexicalConst) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (lexicalConst != NULL) {
        releaseVariableDeclaratorList(lexicalConst->declaratorList);
        free(lexicalConst);
        lexicalConst = NULL;
    }
}

void releaseStatement(Statement * statement) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (statement != NULL) {
        // TODO: Add specific cleanup logic for different statement types
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
        if (statementListItem->declaration != NULL) {
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
