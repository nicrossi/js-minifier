#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>
#include <stdbool.h>

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
typedef struct UpdateOp UpdateOp;
typedef struct VariableDeclarator VariableDeclarator;
typedef struct VariableDeclaratorList VariableDeclaratorList;
typedef struct Expression Expression;

typedef enum {
    ASSIGNMENT,
    DIVISION_EXPRESSION,
    EQUALITY_EXPRESSION,
    EMPTY_EXPRESSION,
    GREATER_EXPRESSION,
    GREAT_EQUAL_EXPRESSION,
    IDENTIFIER,
    INTEGER_EXPRESSION,
    LESS_EXPRESSION,
    LESS_EQUAL_EXPRESSION,
    MULTIPLICATION_EXPRESSION,
    POSTFIX_INCREMENT_EXPR, // expr++
    POSTFIX_DECREMENT_EXPR, // expr--
    PREFIX_INCREMENT_EXPR,  // ++expr
    PREFIX_DECREMENT_EXPR,  // --expr
    STRING_LITERAL_EXPRESSION,
    SUB_EXPRESSION,
    SUM_EXPRESSION,
} ExpressionType;

typedef enum {
    LET_DECLARATION,
    CONST_DECLARATION
} LexicalDeclarationType;

typedef enum {
    EMPTY_FOR_INIT,
    EXPRESSION_FOR_INIT,
    LEXICAL_DECLARATION_FOR_INIT
} ForInitializerType;

typedef enum {
    INCREMENT_OP,
    DECREMENT_OP,
} OperatorType;

typedef enum {
    EXPRESSION_STATEMENT,
    DECLARATION_STATEMENT,
    IF_STATEMENT,
    BLOCK_STATEMENT,
    FOR_STATEMENT,
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
        char * string;
        UpdateOp * updateOp;
        int value;
    };
};

typedef struct {
    ForInitializerType type;
    union {
        Expression * expression;
        LexicalDeclaration * lexicalDeclaration;
    };
} ForInitializer;

typedef struct {
    ForInitializer * initializer;
    Expression * condition;
    Expression * increment;
    Statement * body;
} ForStatement;

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
        ForStatement * forStatement;
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

struct UpdateOp {
    OperatorType operator;
    Expression * operand;
    bool isPostfix;
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
void releaseForInitializer(ForInitializer * forInitializer);
void releaseForStatement(ForStatement * forStatement);
void releaseIfStatement(IfStatement * ifStatement);
void releaseLexicalDeclaration(LexicalDeclaration * lexicalConst);
void releaseProgram(Program * program);
void releaseStatement(Statement * statement);
void releaseStatementList(StatementList * statementList);
void releaseString(char * string);
void releaseStatementListItem(StatementListItem * statementListItem);
void releaseUpdateOp(UpdateOp * updateOp);
void releaseVariableDeclarator(VariableDeclarator * variableDeclarator);
void releaseVariableDeclaratorList(VariableDeclaratorList * variableDeclaratorList);
#endif
