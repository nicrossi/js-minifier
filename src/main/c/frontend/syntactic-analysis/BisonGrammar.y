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
	LexicalConst * lexicalConst;
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
%destructor { releaseLexicalConst($$); } <lexicalConst>
%destructor { releaseStatement($$); } <statement>
%destructor { releaseStatementList($$); } <statementList>
%destructor { releaseStatementListItem($$); } <statementListItem>
%destructor { releaseString($$); } <string>
%destructor { releaseVariableDeclarator($$); } <variableDeclarator>
%destructor { releaseVariableDeclaratorList($$); } <variableDeclaratorList>

/** Terminals. */
%token <integer> INTEGER

%token <string> IDENTIFIER_NAME

%token <token> COMMA
%token <token> CONST_KEYWORD
%token <token> EQUAL
%token <token> UNKNOWN
%token <token> SEMICOLON

/** Non-terminals. */
%type <expression> assignmentExpression
%type <declaration> declaration
%type <expression> expression
%type <lexicalConst> lexicalConst
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

%%
// IMPORTANT: To use λ in the following grammar, use the %empty symbol.
program: statementList												{ $$ = StatementListProgramSemanticAction(currentCompilerState(), $1); }
    ;
statementList: statementList statementListItem                      { $$ = StatementListSemanticAction($1, $2); }
    | %empty                                                        { $$ = EmptyStatementListSemanticAction(); }
    ;
statementListItem: declaration                                      { $$ = DeclarationStatementListItemSemanticAction($1); }
//    | statement                                                     { $$ = StatementStatementListItemSemanticAction($1); }
    ;
declaration: lexicalConst                                           { $$ = LexicalConstDeclarationSemanticAction($1); }
    ;
expression: assignmentExpression                                     { $$ = AssignmentExpressionSemanticAction($1); }
    | expression COMMA assignmentExpression                          { $$ = CommaExpressionSemanticAction($1, $3); }
    ;
variableDeclarator:
    IDENTIFIER_NAME EQUAL assignmentExpression                      { $$ = VariableDeclaratorSemanticAction($1, $3); }
    ;
variableDeclaratorList: variableDeclarator                          { $$ = VariableDeclaratorListSemanticAction($1); }
    | variableDeclaratorList COMMA variableDeclarator               { $$ = AppendVariableDeclaratorListSemanticAction($1, $3); }
    ;
lexicalConst:
    CONST_KEYWORD variableDeclaratorList optionalSemicolon          { $$ = LexicalConstSemanticAction($2); }
    ;
assignmentExpression: INTEGER                                       { $$ = IntegerExpressionSemanticAction($1); }
//    | assignmentExpression EQUAL assignmentExpression               { $$ = ChainedAssignmentSemanticAction($1, $3); }
    ;
optionalSemicolon: SEMICOLON
    | %empty
    ;
// Placeholder for other types of statements
statement: SEMICOLON
    ;
%%
