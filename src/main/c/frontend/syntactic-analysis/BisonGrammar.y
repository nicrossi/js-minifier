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
	Declaration * declaration;
	Expression * expression;
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
%destructor { releaseLexicalDeclaration($$); } <lexicalDeclaration>
%destructor { releaseStatement($$); } <statement>
%destructor { releaseStatementList($$); } <statementList>
%destructor { releaseStatementListItem($$); } <statementListItem>
%destructor { releaseString($$); } <string>
%destructor { releaseVariableDeclarator($$); } <variableDeclarator>
%destructor { releaseVariableDeclaratorList($$); } <variableDeclaratorList>
%destructor { releaseIfStatement($$); } <ifStatement>
/** Terminals. */
%token <integer> INTEGER

%token <string> IDENTIFIER_NAME

%token <token> CLOSE_CURLY_BRACE
%token <token> CLOSE_PARENTHESIS
%token <token> COMMA
%token <token> CONST_KEYWORD
%token <token> ELSE_KEYWORD
%token <token> EQUAL
%token <token> EQUALITY
%token <token> IF_KEYWORD
%token <token> LET_KEYWORD
%token <token> OPEN_CURLY_BRACE
%token <token> OPEN_PARENTHESIS
%token <token> SEMICOLON
%token <token> UNKNOWN

/** Non-terminals. */
%type <expression> assignmentExpression
%type <statementList> block
%type <declaration> declaration
%type <expression> expression
%type <ifStatement> ifStatement
%type <lexicalDeclaration> lexicalDeclaration
%type <program> program
%type <statement> statement
%type <statementList> statementList
%type <statementListItem> statementListItem
%type <variableDeclarator> variableDeclarator
%type <variableDeclaratorList> variableDeclaratorList

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%right EQUAL
%left COMMA
%left EQUALITY
%precedence CLOSE_CURLY_BRACE // avoid conflicts with empty block
%precedence IF_KEYWORD
%precedence ELSE_KEYWORD

// Dangling else, and empty statements. Default behavior is good enough.
%expect 2
%%
// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
program: statementList												{ $$ = StatementListProgramSemanticAction(currentCompilerState(), $1); }
    ;
statementList: statementList statementListItem                      { $$ = StatementListSemanticAction($1, $2); }
    | %empty                                                        { $$ = EmptyStatementListSemanticAction(); }
    ;
statementListItem: declaration                                      { $$ = DeclarationStatementListItemSemanticAction($1); }
    | statement                                                     { $$ = StatementStatementListItemSemanticAction($1); }
    ;
declaration: lexicalDeclaration                                     { $$ = LexicalDeclarationSemanticAction($1); }
    ;
expression: variableDeclarator                                      { $$ = AssignmentExpressionSemanticAction($1); }
    | expression COMMA assignmentExpression                         { $$ = CommaExpressionSemanticAction($1, $3); }
    | expression EQUAL assignmentExpression                         { $$ = ChainedAssignmentSemanticAction($1, $3); }
    | assignmentExpression EQUALITY assignmentExpression            { $$ = EqualityExpressionSemanticAction($1, $3); }
    ;
variableDeclarator:
    IDENTIFIER_NAME EQUAL assignmentExpression                      { $$ = VariableDeclaratorSemanticAction($1, $3); }
    ;
variableDeclaratorList: variableDeclarator                          { $$ = VariableDeclaratorListSemanticAction($1); }
    | variableDeclaratorList COMMA variableDeclarator               { $$ = AppendVariableDeclaratorListSemanticAction($1, $3); }
    ;
lexicalDeclaration:
    CONST_KEYWORD variableDeclaratorList optionalSemicolon          { $$ = CreateLexicalDeclarationSemanticAction(CONST_DECLARATION, $2); }
    | LET_KEYWORD variableDeclaratorList optionalSemicolon          { $$ = CreateLexicalDeclarationSemanticAction(LET_DECLARATION, $2); }
    ;
assignmentExpression: INTEGER                                       { $$ = IntegerExpressionSemanticAction($1); }
    | IDENTIFIER_NAME                                               { $$ = IdentifierExpressionSemanticAction($1); }
    ;
optionalSemicolon: SEMICOLON
    | %empty
    ;
ifStatement:
    IF_KEYWORD OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement %prec ELSE_KEYWORD { $$ = IfSemanticAction($3, $5, NULL); }
    | IF_KEYWORD OPEN_PARENTHESIS expression CLOSE_PARENTHESIS statement ELSE_KEYWORD statement { $$ = IfSemanticAction($3, $5, $7); }
    ;
block:
    OPEN_CURLY_BRACE statementList CLOSE_CURLY_BRACE               { $$ = BlockSemanticAction($2); }
    | OPEN_CURLY_BRACE CLOSE_CURLY_BRACE                           { $$ = EmptyBlockSemanticAction(); }
    ;
statement: expression optionalSemicolon                            { $$ = ExpressionStatementSemanticAction($1); }
    | ifStatement                                                  { $$ = IfStatementSemanticAction($1); }
    | block                                                        { $$ = BlockStatementSemanticAction($1); }
    ;
%%
