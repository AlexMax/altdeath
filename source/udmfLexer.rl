#include <cstdio>
#include <cstdlib>
#include <string>

const char* marked = 0;

void token(const char* token, const char* end) {
	char thisToken[256];

	memcpy(thisToken, marked, end - marked);
	thisToken[end - marked] = '\0';

	printf("Emit: %s (%s)\n", token, thisToken);
}

%%{
	machine UDMF;

	action mark {
		marked = fpc;
	}

	action endAssign {
		printf("End Assign %p (two tokens back = last token)\n", fpc);
	}

	action beginBlock {
		printf("Begin Block %p (last token is block name)\n", fpc);
	}

	action endBlock {
		printf("End Block %p\n", fpc);
	}

	IDENTIFIER = ( [A-Za-z_]+[A-Za-z0-9_]* ) >mark %{ token("IDENTIFIER", fpc); };
	INTEGER = ( [+\-]?[1-9]+[0-9]* | 0[0-9]+ | '0x'.[0-9A-Fa-f]+ ) >mark %{ token("INTEGER", fpc); };
	FLOAT = ( [+\-]?[0-9]+'.'[0-9]*([eE][+\-]?[0-9]+)? ) >mark %{ token("FLOAT", fpc); };
	QUOTED_STRING = ( '"' ( /\\./ | [^"\\] )* '"' ) >mark %{ token("QUOTED_STRING", fpc); };
	KEYWORD = ( 'true' | 'false' ) >mark %{ token("KEYWORD", fpc); };

	C_COMMENT = '/*' any* :>> '*/';
	CPP_COMMENT = '//' any* :>> '\n';
	sc = ( space | C_COMMENT | CPP_COMMENT )*;

	value = INTEGER | FLOAT | QUOTED_STRING | KEYWORD;
	assignment = ( sc IDENTIFIER sc '=' sc value sc ';' ) %endAssign;
	block = ( sc IDENTIFIER ( sc '{' %beginBlock ) sc assignment+ sc '}' ) %endBlock;
	global = sc ( assignment | block )+ sc;

	main := global;
}%%

%% write data;

namespace udmf {

void parse(const std::string& input) {
	int cs;
	const char *p, *pe, *eof;

	size_t len = input.length();
	p = input.c_str();
	pe = eof = p + len;

	%% write init;
	%% write exec;
}

}
