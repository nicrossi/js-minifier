#include "FlexActions.h"
#include "../../shared/ecalloc.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

// Need a buffer to build the string content dynamically.
// Flex's yytext is temporary, so we accumulate the string here.
typedef struct StringBuffer {
    char * buffer;
    size_t size;
    size_t length;
} StringBuffer;

static struct StringBuffer _stringBuffer = {
    .buffer = NULL,
    .size = 0,
    .length = 0
};

static void bufferFits(size_t extraSpace) {
    size_t required = _stringBuffer.length + extraSpace + 1;
    if (_stringBuffer.size < required) {
        size_t newSize = _stringBuffer.size == 0 ? required + 16 : _stringBuffer.size * 2;
        if (newSize < required)
            newSize = required + 16; // If doubling is not enough
        char * newBuffer = realloc(_stringBuffer.buffer, newSize);
        if (newBuffer == NULL) {
            logError(_logger, "Memory allocation failed for string buffer");
            exit(EXIT_FAILURE);
        }
        _stringBuffer.buffer = newBuffer;
        _stringBuffer.size = newSize;
    }
}

static void appendCharToBuffer(char c) {
    bufferFits(1);
    _stringBuffer.buffer[_stringBuffer.length++] = c;
}

static void appendToBuffer(const char * str, size_t length) {
    bufferFits(length);
    memcpy(_stringBuffer.buffer + _stringBuffer.length, str, length);
    _stringBuffer.length += length;
}

static void clearStringBuffer() {
    if (_stringBuffer.buffer != NULL) {
        free(_stringBuffer.buffer);
        _stringBuffer.buffer = NULL;
        _stringBuffer.size = 0;
        _stringBuffer.length = 0;
    }
}

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void BeginDoubleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    _stringBuffer.length = 0; // If buffer NULL or too small, it will be reallocated
}

void BeginSingleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    _stringBuffer.length = 0; // If buffer NULL or too small, it will be reallocated
}

Token EndDoubleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    appendCharToBuffer('\0'); // Null-terminate the accumulated string
    lexicalAnalyzerContext->semanticValue->string = strdup(_stringBuffer.buffer);
    clearStringBuffer();
    if (lexicalAnalyzerContext->semanticValue->string == NULL) {
        logError(_logger, "Memory allocation failed for string");
        exit(EXIT_FAILURE);
    }
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    return STRING_LITERAL;
}

Token EndSingleQuoteLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    appendCharToBuffer('\0'); // Null-terminate the accumulated string
    lexicalAnalyzerContext->semanticValue->string = strdup(_stringBuffer.buffer);
    clearStringBuffer();
    if (lexicalAnalyzerContext->semanticValue->string == NULL) {
        logError(_logger, "Memory allocation failed for string");
        exit(EXIT_FAILURE);
    }
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    // For now both single and double quotes are treated the same.
    return STRING_LITERAL;
}

void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void EscapedCharOrNewlineLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    // Handle escaped characters or newlines
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    appendCharToBuffer(lexicalAnalyzerContext->lexeme[0]);
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void StringLiteralLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    // Handle any other character that is not a backslash, double/single quote (depending on the case), or newline
    // JavaScript double-quoted strings do NOT allow raw newlines; they must be escaped.
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    appendToBuffer(lexicalAnalyzerContext->lexeme, lexicalAnalyzerContext->length);
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	if (_logIgnoredLexemes) {
		_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	}
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
}

Token IdentifierNameLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->string = strdup(lexicalAnalyzerContext->lexeme);
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    return IDENTIFIER_NAME;
}

Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	lexicalAnalyzerContext->semanticValue->integer = atoi(lexicalAnalyzerContext->lexeme);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return INTEGER;
}

Token KeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    return token;
}

Token PunctuationLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token) {
    _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    lexicalAnalyzerContext->semanticValue->token = token;
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    return token;
}

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
	_logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
	destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
	return UNKNOWN;
}

void UnterminatedStringLiteralLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext) {
    if (_logIgnoredLexemes) {
        _logLexicalAnalyzerContext(__FUNCTION__, lexicalAnalyzerContext);
    }
    logError(_logger, "Unterminated string (missing closing quote)");
    clearStringBuffer();
    destroyLexicalAnalyzerContext(lexicalAnalyzerContext);
    exit(EXIT_FAILURE);
}