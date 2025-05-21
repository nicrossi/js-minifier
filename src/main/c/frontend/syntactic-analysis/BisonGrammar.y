%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */
	int integer;
	char * string;
	Token token;

	/** Non-terminals. */
	ArgumentList * argumentList;
	Declaration * declaration;
	Expression * expression;
	ForInitializer * forStatementInit;
	ForStatement * forStatement;
	FunctionDeclaration * functionDeclaration;
	IfStatement * ifStatement;
	LexicalDeclaration * lexicalDeclaration;
	Program * program;
	Statement * statement;
    StatementList * statementList;
    StatementListItem * statementListItem;
    VariableDeclarator * variableDeclarator;
    VariableDeclaratorList * variableDeclaratorList;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseDeclaration($$); } <declaration>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseForInitializer($$); } <forStatementInit>
%destructor { releaseForStatement($$); } <forStatement>
%destructor { releaseIfStatement($$); } <ifStatement>
%destructor { releaseLexicalDeclaration($$); } <lexicalDeclaration>
%destructor { releaseStatement($$); } <statement>
%destructor { releaseStatementList($$); } <statementList>
%destructor { releaseStatementListItem($$); } <statementListItem>
%destructor { releaseString($$); } <string>
%destructor { releaseVariableDeclarator($$); } <variableDeclarator>
%destructor { releaseVariableDeclaratorList($$); } <variableDeclaratorList>
/** Terminals. */
%token <integer> INTEGER

%token <string> IDENTIFIER_NAME
%token <string> STRING_LITERAL

%token <token> BREAK_KEYWORD
%token <token> CONTINUE_KEYWORD
%token <token> CLOSE_CURLY_BRACE
%token <token> CLOSE_PARENTHESIS
%token <token> COMMA
%token <token> CONST_KEYWORD
%token <token> DECREMENT
%token <token> DIVISION
%token <token> ELSE_KEYWORD
%token <token> EQUAL
%token <token> EQUALITY
%token <token> STRICT_EQUALITY
%token <token> FOR_KEYWORD
%token <token> FUNCTION_KEYWORD
%token <token> GREATER GREAT_EQUAL
%token <token> IF_KEYWORD
%token <token> INCREMENT
%token <token> LET_KEYWORD
%token <token> LESS LESS_EQUAL
%token <token> MULTIPLICATION
%token <token> OPEN_CURLY_BRACE
%token <token> OPEN_PARENTHESIS
%token <token> RETURN_KEYWORD
%token <token> SEMICOLON
%token <token> SUB
%token <token> SUM
%token <token> UNKNOWN

/** Non-terminals. */
%type <expression> additiveExpression
%type <argumentList> argumentList
%type <expression> assignmentExpression
%type <statementList> block
%type <declaration> declaration
%type <expression> expression
%type <expression> equalityExpression
%type <forStatementInit> forStatementInit
%type <forStatement> forStatement
%type <functionDeclaration> functionDeclaration
%type <lexicalDeclaration> forLexicalDeclaration
%type <ifStatement> ifStatement
%type <expression> leftHandSideExpression
%type <lexicalDeclaration> lexicalDeclaration
%type <expression> multiplicativeExpression
%type <expression> optionalExpression
%type <variableDeclaratorList> parameterList
%type <expression> primaryExpression
%type <program> program
%type <expression> relationalExpression
%type <statement> statement
%type <statementList> statementList
%type <statementListItem> statementListItem
%type <expression> unaryExpression
%type <variableDeclarator> variableDeclarator
%type <variableDeclaratorList> variableDeclaratorList

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%right EQUAL
%left  EQUALITY STRICT_EQUALITY
%left  LESS_EQUAL GREAT_EQUAL
%left  SUM SUB
%left  MULTIPLICATION
%precedence UNARY
%precedence POSTFIX_UPDATE
%left  INCREMENT DECREMENT
%nonassoc IF_WITHOUT_ELSE
%nonassoc ELSE_KEYWORD

// Dangling else, and empty statements. Default behavior is good enough.
%expect 2
%%
// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
program:
    statementList
        { $$ = StatementListProgramSemanticAction(currentCompilerState(), $1); }
    ;

statementList:
    statementList statementListItem
        { $$ = StatementListSemanticAction($1, $2); }
    | %empty
        { $$ = EmptyStatementListSemanticAction(); }
    ;

statementListItem:
    declaration
        { $$ = DeclarationStatementListItemSemanticAction($1); }
    | statement
        { $$ = StatementStatementListItemSemanticAction($1); }
    ;
declaration:
    lexicalDeclaration
        { $$ = LexicalDeclarationSemanticAction($1); }
    | functionDeclaration
        { $$ = FunctionDeclarationSemanticAction($1); }
    ;

lexicalDeclaration:
    CONST_KEYWORD variableDeclaratorList optionalSemicolon
        { $$ = CreateLexicalDeclarationSemanticAction(CONST_DECLARATION, $2); }
    | LET_KEYWORD variableDeclaratorList optionalSemicolon
        { $$ = CreateLexicalDeclarationSemanticAction(LET_DECLARATION, $2); }
    ;

functionDeclaration:
    FUNCTION_KEYWORD IDENTIFIER_NAME OPEN_PARENTHESIS parameterList CLOSE_PARENTHESIS block
        { $$ = FunctionSemanticAction($2, $4, $6); }
    ;

parameterList:
    %empty
        { $$ = EmptyParameterListSemanticAction(); }
    | variableDeclarator
        { $$ = VariableDeclaratorListSemanticAction($1); }
    | parameterList COMMA variableDeclarator
        { $$ = AppendParameterListSemanticAction($1, $3); }
    ;

variableDeclaratorList:
    variableDeclarator
        { $$ = VariableDeclaratorListSemanticAction($1); }
    | variableDeclaratorList COMMA variableDeclarator
        { $$ = AppendVariableDeclaratorListSemanticAction($1, $3); }
    ;

variableDeclarator:
    IDENTIFIER_NAME EQUAL assignmentExpression
        { $$ = VariableDeclaratorSemanticAction($1, $3); }
    | IDENTIFIER_NAME
        { $$ = VariableDeclaratorSemanticAction($1, NULL); }
    ;

statement:
    block
        { $$ = BlockStatementSemanticAction($1); }
    | expression SEMICOLON // Optional semicolon was making my life difficult, so it's gone. For now...
        { $$ = ExpressionStatementSemanticAction($1); }
    | BREAK_KEYWORD SEMICOLON
        { $$ = BreakStatementSemanticAction(); }
    |  CONTINUE_KEYWORD SEMICOLON
        { $$ = ContinueStatementSemanticAction(); }
    | RETURN_KEYWORD optionalExpression SEMICOLON
        { $$ = ReturnStatementSemanticAction($2); }
    | ifStatement
        { $$ = IfStatementSemanticAction($1); }
    | forStatement
        { $$ = ForStatementSemanticAction($1); }
    ;

optionalSemicolon: SEMICOLON | %empty ;

block:
    OPEN_CURLY_BRACE statementList CLOSE_CURLY_BRACE
        { $$ = BlockSemanticAction($2); }
    ;

expression:
    expression COMMA assignmentExpression
        { $$ = CommaExpressionSemanticAction($1, $3); }
    | assignmentExpression
    ;

assignmentExpression:
    leftHandSideExpression EQUAL assignmentExpression
        { $$ = AssignmentExpressionSemanticAction($1, $3); }
    | equalityExpression
    ;

equalityExpression:
    equalityExpression EQUALITY additiveExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, EQUALITY_EXPRESSION); }
    | equalityExpression STRICT_EQUALITY additiveExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, STRICT_EQUALITY_EXPRESSION); }
    | relationalExpression
    ;

relationalExpression:
    relationalExpression LESS_EQUAL additiveExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, LESS_EQUAL_EXPRESSION); }
    | relationalExpression GREAT_EQUAL additiveExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, GREAT_EQUAL_EXPRESSION); }
    | relationalExpression LESS additiveExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, LESS_EXPRESSION); }
    | relationalExpression GREATER additiveExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, GREATER_EXPRESSION); }
    | additiveExpression
    ;

additiveExpression:
    additiveExpression SUM multiplicativeExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, SUM_EXPRESSION); }
    | additiveExpression SUB multiplicativeExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, SUB_EXPRESSION); }
    | multiplicativeExpression
    ;

multiplicativeExpression:
    multiplicativeExpression MULTIPLICATION unaryExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, MULTIPLICATION_EXPRESSION); }
    | multiplicativeExpression DIVISION unaryExpression
        { $$ = BinaryExpressionSemanticAction($1, $3, DIVISION_EXPRESSION); }
    | unaryExpression
    ;

unaryExpression:
    INCREMENT leftHandSideExpression
        { $$ = UnaryExpressionSemanticAction($2, INCREMENT_OP, false); }
    | DECREMENT leftHandSideExpression
        { $$ = UnaryExpressionSemanticAction($2, DECREMENT_OP, false); }
    | leftHandSideExpression INCREMENT
        { $$ = UnaryExpressionSemanticAction($1, INCREMENT_OP, true); }
    | leftHandSideExpression DECREMENT
        { $$ = UnaryExpressionSemanticAction($1, DECREMENT_OP, true); }
    | leftHandSideExpression %prec UNARY
    | primaryExpression
    ;

leftHandSideExpression:
    IDENTIFIER_NAME
        { $$ = IdentifierExpressionSemanticAction($1); }
    ;

primaryExpression:
    INTEGER
        { $$ = IntegerExpressionSemanticAction($1); }
    | STRING_LITERAL
        { $$ = StringExpressionSemanticAction($1); }
    | leftHandSideExpression OPEN_PARENTHESIS  argumentList CLOSE_PARENTHESIS
        { $$ = CallExpressionSemanticAction($1, $3); }
    | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
        { $$ = ParenthesisExpressionSemanticAction($2); }
    ;

argumentList:
    %empty
        { $$ = EmptyArgumentListSemanticAction(); }
    | assignmentExpression
        { $$ = ArgumentListSemanticAction($1); }
    | argumentList COMMA assignmentExpression
        { $$ = AppendArgumentListSemanticAction($1, $3); }
    ;

ifStatement:
    IF_KEYWORD OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement %prec IF_WITHOUT_ELSE
        { $$ = IfSemanticAction($3, $5, NULL); }
    | IF_KEYWORD OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement ELSE_KEYWORD statement
        { $$ = IfSemanticAction($3, $5, $7); }
    ;

forStatement:
    FOR_KEYWORD OPEN_PARENTHESIS forStatementInit SEMICOLON optionalExpression SEMICOLON optionalExpression CLOSE_PARENTHESIS statement
      { $$ = ForIterationSemanticAction($3, $5, $7, $9);}
   ;

forStatementInit:
    %empty
        { $$ = EmptyForInitSemanticAction(); }
    | expression
        { $$ = ExpressionForInitSemanticAction($1); }
    | forLexicalDeclaration
        { $$ = LexicalDeclarationForInitSemanticAction($1); }
    ;

forLexicalDeclaration:
    CONST_KEYWORD variableDeclaratorList
        { $$ = CreateLexicalDeclarationSemanticAction(CONST_DECLARATION, $2); }
    | LET_KEYWORD variableDeclaratorList
        { $$ = CreateLexicalDeclarationSemanticAction(LET_DECLARATION, $2); }
    ;

optionalExpression:
    %empty
        { $$ = EmptyExpressionSemanticAction(); }
    | expression
        { $$ = OptionalExpression($1); }
    ;
%%
