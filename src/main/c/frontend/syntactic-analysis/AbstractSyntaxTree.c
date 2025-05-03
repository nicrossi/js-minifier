#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseDeclaration(Declaration * declaration) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (declaration != NULL) {
        releaseLexicalConst(declaration->lexicalConst);
        free(declaration);
    }
}

void releaseLexicalConst(LexicalConst * lexicalConst) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (lexicalConst != NULL) {
        free(lexicalConst->identifierName);
        releaseConstant(lexicalConst->constant);
        free(lexicalConst);
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
    }
}

void releaseString(char * string) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    free(string);
}

void releaseProgram(Program * program) {
    logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (program != NULL) {
        releaseStatementList(program->statementList);
        free(program);
    }
}
