%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"const"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), CONST_KEYWORD); }
"let"                               { return KeywordLexemeAction(createLexicalAnalyzerContext(), LET_KEYWORD); }
"if"                                { return KeywordLexemeAction(createLexicalAnalyzerContext(), IF_KEYWORD); }
"else"                              { return KeywordLexemeAction(createLexicalAnalyzerContext(), ELSE_KEYWORD); }

"="                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), EQUAL); }
"=="                                { return PunctuationLexemeAction(createLexicalAnalyzerContext(), EQUALITY); }
";"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
","                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), COMMA); }
"("                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
"{"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), OPEN_CURLY_BRACE); }
"}"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), CLOSE_CURLY_BRACE); }

[a-zA-Z_\\$][a-zA-Z0-9_\\$]*        { return IdentifierNameLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
