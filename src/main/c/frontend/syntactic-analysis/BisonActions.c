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

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}

LexicalConst * LexicalConstSemanticAction(const char * string, Constant * constant) {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    LexicalConst * lexicalConst = ecalloc(1, sizeof(LexicalConst));
    lexicalConst->identifierName = strdup(string);
    free((char *) string);
    lexicalConst->constant = constant;
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
