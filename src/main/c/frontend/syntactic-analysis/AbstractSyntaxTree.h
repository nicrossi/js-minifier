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
typedef struct Argument Argument;
typedef struct ArgumentList ArgumentList;
typedef struct Declaration Declaration;
typedef struct LexicalDeclaration LexicalDeclaration;
typedef struct FunctionDeclaration FunctionDeclaration;
typedef struct Program Program;
typedef struct Statement Statement;
typedef struct StatementList StatementList;
typedef struct StatementListItem StatementListItem;
typedef struct TryStatement TryStatement;
typedef struct UpdateOp UpdateOp;
typedef struct VariableDeclarator VariableDeclarator;
typedef struct VariableDeclaratorList VariableDeclaratorList;
typedef struct Expression Expression;

typedef enum {
    LEXICAL,
    FUNCTION,
} DeclarationType;

typedef enum {
    ASSIGNMENT,
    CALL_EXPRESSION,
    DIVISION_EXPRESSION,
    EQUALITY_EXPRESSION,
    EMPTY_EXPRESSION,
    EXPONENTIATION_EXPRESSION,
    GREATER_EXPRESSION,
    GREAT_EQUAL_EXPRESSION,
    IDENTIFIER,
    INEQUALITY_EXPRESSION,
    INTEGER_EXPRESSION,
    LESS_EXPRESSION,
    LESS_EQUAL_EXPRESSION,
    MULTIPLICATION_EXPRESSION,
    POSTFIX_INCREMENT_EXPR, // expr++
    POSTFIX_DECREMENT_EXPR, // expr--
    POWER_EXPRESSION,
    PREFIX_INCREMENT_EXPR,  // ++expr
    PREFIX_DECREMENT_EXPR,  // --expr
    REMAINDER_EXPRESSION,
    STRICT_EQUALITY_EXPRESSION,
    STRICT_INEQUALITY_EXPRESSION,
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
    BITWISE_NOT_OP,
    DECREMENT_OP,
    INCREMENT_OP,
    LOGICAL_NOT_OP,
    UNARY_SUM_OP,
    UNARY_SUB_OP,
} OperatorType;

typedef enum {
    BLOCK_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    DECLARATION_STATEMENT,
    EXPRESSION_STATEMENT,
    FOR_STATEMENT,
    IF_STATEMENT,
    RETURN_STATEMENT,
    TRY_STATEMENT,
} StatementType;

/**
 * Node types for the Abstract Syntax Tree (AST).
 */
struct Argument {
    Expression * expression;
    Argument * next;
};

struct ArgumentList {
    Argument * head;
    Argument * tail;
};

typedef struct {
    Expression * leftExpression;
    Expression * rightExpression;
} BinaryExpression;

struct Declaration {
    DeclarationType type;
    union {
        LexicalDeclaration * lexicalDeclaration;
        FunctionDeclaration * functionDeclaration;
    };
};

typedef struct {
    Expression * callee;
    ArgumentList * argumentList;
} CallExpression;

typedef struct {
    /* NULL ⇒ `catch { … }` (parameter-less catch – ES2019) */
    char * identifier;
    StatementList * block;
} CatchClause;

struct Expression {
    ExpressionType type;
    union {
        BinaryExpression binaryExpression;
        CallExpression * callExpression;
        char * identifierName;
        char * string;
        UpdateOp * updateOp;
        int value;
    };
};

typedef struct {
    StatementList * block;
} FinallyClause;

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

struct FunctionDeclaration {
    char * identifier;
    VariableDeclaratorList * parameterList;
    StatementList * body;
};

struct Statement {
    StatementType type;
    union {
        Expression * expression;
        StatementList * block;
        IfStatement * ifStatement;
        ForStatement * forStatement;
        TryStatement * tryStatement;
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

struct TryStatement {
    StatementList * tryBlock;
    CatchClause * catchClause;
    FinallyClause * finallyClause;
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
void releaseArgument(Argument * argument);
void releaseArgumentList(ArgumentList * argumentList);
void releaseCallExpression(CallExpression * callExpression);
void releaseCatchClause(CatchClause * catchClause);
void releaseDeclaration(Declaration * declaration);
void releaseExpression(Expression * expression);
void releaseFinallyClause(FinallyClause * finallyClause);
void releaseFunctionDeclaration(FunctionDeclaration * functionDeclaration);
void releaseForInitializer(ForInitializer * forInitializer);
void releaseForStatement(ForStatement * forStatement);
void releaseIfStatement(IfStatement * ifStatement);
void releaseLexicalDeclaration(LexicalDeclaration * lexicalConst);
void releaseProgram(Program * program);
void releaseStatement(Statement * statement);
void releaseStatementList(StatementList * statementList);
void releaseString(char * string);
void releaseStatementListItem(StatementListItem * statementListItem);
void releaseTryStatement(TryStatement * tryStatement);
void releaseUpdateOp(UpdateOp * updateOp);
void releaseVariableDeclarator(VariableDeclarator * variableDeclarator);
void releaseVariableDeclaratorList(VariableDeclaratorList * variableDeclaratorList);
#endif
