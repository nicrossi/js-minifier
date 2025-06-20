#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
    initializeExpressionGeneratorModule();
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
    shutdownExpressionGeneratorModule();
}

/** PRIVATE FUNCTIONS */
typedef void (* StatementGenFn) (const Statement * statement);
typedef void (* DeclarationGenFn) (const Declaration * declaration);

static void _genProgram(Program * program);
static char * _indentation(const unsigned int indentationLevel);

static void _genStatementList(const StatementList * statementList);
static void _genStatementListItem(const StatementListItem * sli);
static void _genStatement(const Statement * stmt);
static void _genBlockStatement(const Statement * st);
static void _genBreakStatement(const Statement * st);
static void _genContinueStatement(const Statement * st);
static void _genDeclarationStatement(const StatementListItem * sli);
static void _genDoWhileStatement(const Statement * st);
static void _genExpressionStatement(const Statement * sli);
static void _genForStatement(const Statement * st);
static void _genIfStatement(const Statement * st);
static void _genReturnStatement(const Statement * st);
static void _genThrowStatement(const Statement * st);
static void _genTryStatement(const Statement * st);
static void _genSwitchStatement(const Statement * st);
static void _genWhileStatement(const Statement * st);

static void _emitDeclaration(const Declaration * declaration);
static void _emitLexicalDeclaration(const Declaration * declaration);
static void _emitFunctionDeclaration(const Declaration * declaration);

static const StatementGenFn _statementGenTable[] = {
        // [StatementType] = handlerFunction
        [BLOCK_STATEMENT] = _genBlockStatement,
//        [BREAK_STATEMENT] = _genBreakStatement,
//        [CONTINUE_STATEMENT] = _genContinueStatement,
        [DECLARATION_STATEMENT] = NULL, // Handled separately
//        [DO_WHILE_STATEMENT] = _genDoWhileStatement,
        [EXPRESSION_STATEMENT] = _genExpressionStatement,
//        [FOR_STATEMENT] = _genForStatement,
        [IF_STATEMENT] = _genIfStatement,
//        [RETURN_STATEMENT] = _genReturnStatement,
//        [THROW_STATEMENT] = _genThrowStatement,
//        [TRY_STATEMENT] = _genTryStatement,
//        [SWITCH_STATEMENT] = _genSwitchStatement,
//        [WHILE_STATEMENT] = _genWhileStatement
};

static const DeclarationGenFn _declarationGenTable[] = {
        // [DeclarationType] = handlerFunction
        [LEXICAL] = _emitLexicalDeclaration,
//        [FUNCTION] = _emitFunctionDeclaration,
};

/* Generates the output of the program. */
static void _genProgram(Program * program) {
    _genStatementList(program->statementList);
}

/* Generates the output of a statement list. */
static void _genStatementList(const StatementList * statementList) {
    for (const StatementListItem * it = statementList->head; it; it = it->next) {
        _genStatementListItem(it);
    }
}

/* Generate declaration statement */
static void _genDeclarationStatement(const StatementListItem * sli) {
    if (sli->declaration == NULL) {
        logError(_logger, "Attempt to generate output for a NULL declaration statement.");
        return;
    }
    logDebugging(_logger, "Generating output for declaration statement.");
    _emitDeclaration(sli->declaration);
}

/* Generates the output of a statement. */
static void _genStatementListItem(const StatementListItem * sli) {
    if (sli == NULL) {
        logDebugging(_logger, "Attempt to generate output for a NULL statement.");
        return;
    }

    logDebugging(_logger, "Generating output for statement of type: %d", sli->type);
    if (sli->type >= 0 && sli->type < ARRAY_LEN(_statementGenTable)) {
        sli->type == DECLARATION_STATEMENT
        ? _genDeclarationStatement(sli) : _genStatement(sli->statement);
    } else {
        logError(_logger, "Unknown statement type: %d", sli->type);
    }
}

/* Generate statement */
static void _genStatement(const Statement * stmt) {
    _statementGenTable[stmt->type](stmt);
}

static void _emitDeclaration(const Declaration * declaration) {
    logDebugging(_logger, "Generating output for declaration of type: %d",
                 declaration->type);
    (declaration->type < ARRAY_LEN(_declarationGenTable)
     && _declarationGenTable[declaration->type] != NULL)
        ? _declarationGenTable[declaration->type](declaration)
        : logError(_logger, "Unknown declaration type: %d", declaration->type);
}

static void _emitLexicalDeclaration(const Declaration * declaration) {
    if (declaration == NULL || declaration->lexicalDeclaration == NULL) {
        logError(_logger, "Attempt to generate output for a NULL lexical declaration.");
        return;
    }
    logDebugging(_logger, "Generating output for lexical declaration.");
    const char * keyword = (declaration->lexicalDeclaration->type == LET_DECLARATION) ? "let" : "const";
    EMIT("%s ", keyword);

    VariableDeclaratorList * declaratorList = declaration->lexicalDeclaration->declaratorList;
    for (const VariableDeclarator * vd = declaratorList->head; vd; vd = vd->next) {
        EMIT("%s", vd->identifier);
        if (vd->initializer != NULL) {
            EMIT("=");
            genExpression(vd->initializer);
        }
        if (vd->next != NULL) {
            EMIT(",");
        }
    }

    EMIT(";");
}

static void _genExpressionStatement(const Statement * stmt) {
    if (stmt == NULL || stmt->expression == NULL) {
        logError(_logger, "Attempt to generate output for a NULL expression statement.");
        return;
    }

    logDebugging(_logger, "Generating output for expression statement.");
    genExpression(stmt->expression);
    EMIT(";");
}

static void _genIfStatement(const Statement * stmt) {
    if (stmt == NULL || stmt->ifStatement == NULL) {
        logError(_logger, "Attempt to generate output for a NULL if statement.");
        return;
    }

    logDebugging(_logger, "Generating output for if statement.");
    EMIT("if(");
    genExpression(stmt->ifStatement->condition);
    EMIT(")");
    _genStatement(stmt->ifStatement->thenStatement);

    if (stmt->ifStatement->elseStatement != NULL) {
        EMIT("else");
        _genStatement(stmt->ifStatement->elseStatement);
    }
}

static void _genBlockStatement(const Statement * st) {
    if (st == NULL || st->block == NULL) {
        logError(_logger, "Attempt to generate output for a NULL block statement.");
        return;
    }

    logDebugging(_logger, "Generating output for block statement.");
    EMIT("{");
    _genStatementList(st->block);
    EMIT("}");
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/** PUBLIC FUNCTIONS */
/**
 * Outputs a formatted string to output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
void output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
    FILE * out = getOutputStream();
	vfprintf(out, effectiveFormat, arguments);
	fflush(out);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_genProgram(compilerState->abstractSyntaxtTree);
	logDebugging(_logger, "Generation is done.");
}
