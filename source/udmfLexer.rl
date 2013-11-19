#include <cstdio>
#include <cstdlib>
#include <string>

#include "udmfParser.h"

extern "C" {

extern void UDMF_ParseTrace(FILE *TraceFILE, char *zTracePrompt);
extern void *UDMF_ParseAlloc(void *(*mallocProc)(size_t));
extern void UDMF_Parse(void *yyp, int yymajor, void* yyminor);
extern void UDMF_ParseFree(void *p, void (*freeProc)(void*));

}

static void parseString(void* parser, int type, const char* ts, const char* te) {
	size_t length = te - ts;
	char* token = static_cast<char*>(malloc(length + 1));
	memcpy(token, ts, length);
	token[length] = 0;
	printf("%s\n", token);
	UDMF_Parse(parser, type, token);
	free(token);
}

%%{
	machine UDMF;

	IDENTIFIER = [A-Za-z_]+[A-Za-z0-9_]*;
	LEFT_PAREN = '{';
	RIGHT_PAREN = '}';
	EQUALS = '=';
	SEMICOLON = ';';
	INTEGER = [+\-]?[1-9]+[0-9]* | 0[0-9]+ | '0x'.[0-9A-Fa-f]+;
	FLOAT = [+\-]?[0-9]+'.'[0-9]*([eE][+\-]?[0-9]+)?;

	QS_quote = '"';
	QS_escape =  /\\./;
	QS_else = [^"\\];

	KEYWORD = 'true' | 'false';

	action identifierToken {
		parseString(parser, UDMF_IDENTIFIER, ts, te);
	}

	action leftParenToken {
		UDMF_Parse(parser, UDMF_LEFT_PAREN, 0);
	}

	action rightParenToken {
		UDMF_Parse(parser, UDMF_RIGHT_PAREN, 0);
	}

	action equalsToken {
		UDMF_Parse(parser, UDMF_EQUALS, 0);
	}

	action semicolonToken {
		UDMF_Parse(parser, UDMF_SEMICOLON, 0);
	}

	action integerToken {
		parseString(parser, UDMF_INTEGER, ts, te);
	}

	action floatToken {
		parseString(parser, UDMF_FLOAT, ts, te);
	}

	action quotedStringToken {
		parseString(parser, UDMF_QUOTED_STRING, ts, te);
	}

	action keywordToken {
		parseString(parser, UDMF_KEYWORD, ts, te);
	}

	main := |*
		IDENTIFIER => identifierToken;
		LEFT_PAREN => leftParenToken;
		RIGHT_PAREN => rightParenToken;
		EQUALS => equalsToken;
		SEMICOLON => semicolonToken;
		INTEGER => integerToken;
		FLOAT => floatToken;
		QS_quote ( QS_escape | QS_else )* QS_quote => quotedStringToken;
		KEYWORD => keywordToken;
		space;
	*|;
}%%

%% write data;

namespace udmf {

void parse(const std::string& input) {
	int cs, act;
	const char *p, *pe, *eof, *ts, *te;

	size_t len = input.length();
	p = input.c_str();
	pe = eof = p + len + 1;

	UDMF_ParseTrace(stderr, "UDMF_Parse:");
	void* parser = UDMF_ParseAlloc(malloc);

	%% write init;
	%% write exec;

	UDMF_Parse(parser, 0, 0);
	UDMF_ParseFree(parser, free);
}

}
