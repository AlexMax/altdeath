%%{
	machine UDMF;

	IDENTIFIER = [A-Za-z_]+[A-Za-z0-9_]*;
	LEFT_PAREN = '{';
	RIGHT_PAREN = '}';
	EQUALS = '=';
	SEMICOLON = ';';
	INTEGER = [+\-]?[1-9]+[0-9]* | 0[0-9]+ | '0x'.[0-9A-Fa-f]+;
	FLOAT = [+\-]?[0-9]+'.'[0-9]*([eE][+\-]?[0-9]+)?;
	QUOTED_STRING = "(.*)";
	KEYWORD = [^{}();"'\n\t ]+;

	action identifierToken {
		UDMF_Parse(parser, UDMF_IDENTIFIER, "Identifier");
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
		UDMF_Parse(parser, UDMF_INTEGER, 0);
	}

	action floatToken {
		UDMF_Parse(parser, UDMF_FLOAT, 0);
	}

	action quotedStringToken {
		UDMF_Parse(parser, UDMF_QUOTED_STRING, 0);
	}

	action keywordToken {
		UDMF_Parse(parser, UDMF_KEYWORD, 0);
	}

	main := |*
		IDENTIFIER => identifierToken;
		LEFT_PAREN => leftParenToken;
		RIGHT_PAREN => rightParenToken;
		EQUALS => equalsToken;
		SEMICOLON => semicolonToken;
		INTEGER => integerToken;
		FLOAT => floatToken;
		QUOTED_STRING => quotedStringToken;
		KEYWORD => keywordToken;
	*|;
}%%

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "udmfParser.h"

%% write data;

extern void UDMF_ParseTrace(FILE *TraceFILE, char *zTracePrompt);
extern void *UDMF_ParseAlloc(void* (*mallocProc)(size_t));
extern void UDMF_Parse(void* yyp, int yymajor, void* yyminor);
extern void UDMF_ParseFree(void* p, void (*freeProc)(void*));

void UDMF_Scan(const char* p) {
	int cs, act;
	char* ts, te;

	size_t len = strlen(p);
	const char* pe = p + len;
	const char* eof = p + len;

	UDMF_ParseTrace(stderr, "UDMF_Parse:");
	void* parser = UDMF_ParseAlloc(malloc);

	%% write init;
	%% write exec;

	UDMF_Parse(parser, 0, 0);
	UDMF_ParseFree(parser, free);
}
