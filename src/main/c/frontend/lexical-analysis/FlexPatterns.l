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
%x STRING_DOUBLE_QUOTE
%x STRING_SINGLE_QUOTE
%x SINGLE_LINE_COMMENT

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

"//"                                { BEGIN(SINGLE_LINE_COMMENT); IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_LINE_COMMENT>\n             { BEGIN(INITIAL); IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<SINGLE_LINE_COMMENT>.              { IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"\""                                { BEGIN(STRING_DOUBLE_QUOTE); BeginDoubleQuoteLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\"             { Token t = EndDoubleQuoteLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); return t; }
<STRING_DOUBLE_QUOTE>\\n            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\t            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\r            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\b            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\f            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\\"           { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\\\           { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\\(.|\n)       { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>[^\\\n\"]+     { StringLiteralLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE>\n             { UnterminatedStringLiteralLexemeAction(createLexicalAnalyzerContext()); }
<STRING_DOUBLE_QUOTE><<EOF>>        { UnterminatedStringLiteralLexemeAction(createLexicalAnalyzerContext()); }

 "'"                                 { BEGIN(STRING_SINGLE_QUOTE); BeginSingleQuoteLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\'             { Token t = EndSingleQuoteLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); return t; }
 <STRING_SINGLE_QUOTE>\\n            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\t            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\r            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\b            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\f            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\'            { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\\\           { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\\(.|\n)       { EscapedCharOrNewlineLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>[^\\\n\']+     { StringLiteralLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE>\n             { UnterminatedStringLiteralLexemeAction(createLexicalAnalyzerContext()); }
 <STRING_SINGLE_QUOTE><<EOF>>        { UnterminatedStringLiteralLexemeAction(createLexicalAnalyzerContext()); }

"const"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), CONST_KEYWORD); }
"let"                               { return KeywordLexemeAction(createLexicalAnalyzerContext(), LET_KEYWORD); }
"if"                                { return KeywordLexemeAction(createLexicalAnalyzerContext(), IF_KEYWORD); }
"else"                              { return KeywordLexemeAction(createLexicalAnalyzerContext(), ELSE_KEYWORD); }
"for"                               { return KeywordLexemeAction(createLexicalAnalyzerContext(), FOR_KEYWORD); }
"break"                             { return KeywordLexemeAction(createLexicalAnalyzerContext(), BREAK_KEYWORD); }
"continue"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), CONTINUE_KEYWORD); }
"return"                            { return KeywordLexemeAction(createLexicalAnalyzerContext(), RETURN_KEYWORD); }
"function"                          { return KeywordLexemeAction(createLexicalAnalyzerContext(), FUNCTION_KEYWORD); }

"="                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), EQUAL); }
"=="                                { return PunctuationLexemeAction(createLexicalAnalyzerContext(), EQUALITY); }
";"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
","                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), COMMA); }
"("                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }
"{"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), OPEN_CURLY_BRACE); }
"}"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), CLOSE_CURLY_BRACE); }
"++"                                { return PunctuationLexemeAction(createLexicalAnalyzerContext(), INCREMENT); }
"--"                                { return PunctuationLexemeAction(createLexicalAnalyzerContext(), DECREMENT); }
">="                                { return PunctuationLexemeAction(createLexicalAnalyzerContext(), GREAT_EQUAL); }
">"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), GREATER); }
"<"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), LESS); }
"<="                                { return PunctuationLexemeAction(createLexicalAnalyzerContext(), LESS_EQUAL); }
"-"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), SUB); }
"+"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), SUM); }
"*"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), MULTIPLICATION); }
"/"                                 { return PunctuationLexemeAction(createLexicalAnalyzerContext(), DIVISION); }

[a-zA-Z_\\$][a-zA-Z0-9_\\$]*        { return IdentifierNameLexemeAction(createLexicalAnalyzerContext()); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }

%%

#include "FlexExport.h"
