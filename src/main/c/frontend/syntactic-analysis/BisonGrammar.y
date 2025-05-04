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
	Constant * constant;
	Declaration * declaration;
	LexicalConst * lexicalConst;
	Program * program;
	Statement * statement;
    StatementList * statementList;
    StatementListItem * statementListItem;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseConstant($$); } <constant>
%destructor { releaseDeclaration($$); } <declaration>
%destructor { releaseLexicalConst($$); } <lexicalConst>
%destructor { releaseStatement($$); } <statement>
%destructor { releaseStatementList($$); } <statementList>
%destructor { releaseStatementListItem($$); } <statementListItem>
%destructor { releaseString($$); } <string>

/** Terminals. */
%token <integer> INTEGER

%token <string> IDENTIFIER_NAME

%token <token> CONST_KEYWORD
%token <token> EQUAL
%token <token> UNKNOWN
%token <token> SEMICOLON

/** Non-terminals. */
%type <constant> constant
%type <declaration> declaration
%type <lexicalConst> lexicalConst
%type <program> program
%type <statement> statement
%type <statementList> statementList
%type <statementListItem> statementListItem

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
    | statement                                                     { $$ = StatementStatementListItemSemanticAction($1); }
    ;
declaration: lexicalConst                                           { $$ = LexicalConstDeclarationSemanticAction($1); }
    ;
lexicalConst:
    CONST_KEYWORD IDENTIFIER_NAME EQUAL constant optionalSemicolon  { $$ = LexicalConstSemanticAction($2, $4); }
    ;
optionalSemicolon: SEMICOLON
    | %empty
    ;
// Placeholder for other types of statements
statement: SEMICOLON
    ;
constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;
%%
