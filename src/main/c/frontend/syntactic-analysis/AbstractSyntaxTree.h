#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */


typedef struct Constant Constant;
typedef struct Declaration Declaration;
typedef struct LexicalConst LexicalConst;
typedef struct Program Program;
typedef struct Statement Statement;
typedef struct StatementList StatementList;
typedef struct StatementListItem StatementListItem;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

struct Constant {
	int value;
};

struct Declaration {
    LexicalConst * lexicalConst;
};

struct LexicalConst {
    char * identifierName;
    Constant * constant;
};

struct Statement {
    // Placeholder for different types of statements
    int type;
};

struct StatementList {
    StatementListItem * head;
    StatementListItem * tail;
};

struct StatementListItem {
    union {
        Declaration * declaration;
        Statement * statement;
    };
    StatementListItem * next;
};

struct Program {
    StatementList * statementList;
};

/**
 * Node recursive destructors.
 */
void releaseConstant(Constant * constant);
void releaseDeclaration(Declaration * declaration);
void releaseLexicalConst(LexicalConst * lexicalConst);
void releaseProgram(Program * program);
void releaseStatement(Statement * statement);
void releaseStatementList(StatementList * statementList);
void releaseStatementListItem(StatementListItem * statementListItem);
void releaseString(char * string);
#endif
