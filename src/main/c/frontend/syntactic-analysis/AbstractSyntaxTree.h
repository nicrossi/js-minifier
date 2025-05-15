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

typedef struct Declaration Declaration;
typedef struct LexicalDeclaration LexicalDeclaration;
typedef struct Program Program;
typedef struct Statement Statement;
typedef struct StatementList StatementList;
typedef struct StatementListItem StatementListItem;
typedef struct VariableDeclarator VariableDeclarator;
typedef struct VariableDeclaratorList VariableDeclaratorList;
typedef struct Expression Expression;

typedef enum {
    ASSIGNMENT,
    IDENTIFIER,
    INTEGER_EXPRESSION,
    BOOLEAN_EXPRESSION,
} ExpressionType;

typedef enum {
    LET_DECLARATION,
    CONST_DECLARATION
} LexicalDeclarationType;

typedef enum {
    EXPRESSION_STATEMENT,
    DECLARATION_STATEMENT,
    IF_STATEMENT,
    BLOCK_STATEMENT,
} StatementType;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */
typedef struct {
    Expression * leftExpression;
    Expression * rightExpression;
} BinaryExpression;

struct Declaration {
    LexicalDeclaration * lexicalDeclaration;
};

struct Expression {
    ExpressionType type;
    union {
        BinaryExpression binaryExpression;
        char * identifierName;
        int value;
    };
};

typedef struct {
    Expression * condition;
    Statement * thenStatement;
    Statement * elseStatement;
} IfStatement;

struct LexicalDeclaration {
    LexicalDeclarationType type;
    VariableDeclaratorList * declaratorList;
};

struct Statement {
    StatementType type;
    union {
        Expression * expression;
        StatementList * block;
        IfStatement * ifStatement;
    };
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
    StatementType type;
};

struct VariableDeclarator {
    char * identifier;
    Expression * initializer;
    VariableDeclarator * next;
};

struct VariableDeclaratorList {
    VariableDeclarator * head;
    VariableDeclarator * tail;
};

struct Program {
    StatementList * statementList;
};

/**
 * Node recursive destructors.
 */
void releaseDeclaration(Declaration * declaration);
void releaseExpression(Expression * expression);
void releaseIfStatement(IfStatement * ifStatement);
void releaseLexicalDeclaration(LexicalDeclaration * lexicalConst);
void releaseProgram(Program * program);
void releaseStatement(Statement * statement);
void releaseStatementList(StatementList * statementList);
void releaseString(char * string);
void releaseStatementListItem(StatementListItem * statementListItem);
void releaseVariableDeclarator(VariableDeclarator * variableDeclarator);
void releaseVariableDeclaratorList(VariableDeclaratorList * variableDeclaratorList);
#endif
