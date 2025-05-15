#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginDoubleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token EndDoubleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EscapedCharOrNewlineLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void StringLiteralLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IdentifierNameLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token KeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token PunctuationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void UnterminatedStringDoubleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
#endif
