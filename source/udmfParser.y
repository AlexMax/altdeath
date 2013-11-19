%include {
	#include <assert.h>
}
%name UDMF_Parse
%token_prefix UDMF_

translation_unit ::= global_expr_list.
global_expr_list ::= global_expr_list global_expr.
global_expr_list ::= .
global_expr ::= block.
global_expr ::= assignment_expr.

block ::= IDENTIFIER(A) LEFT_PAREN expr_list RIGHT_PAREN. {
	printf("Block %s =>\n", A);
}

expr_list ::= expr_list assignment_expr.
expr_list ::= .

assignment_expr ::= IDENTIFIER(A) EQUALS value SEMICOLON. {
	printf("%s = ...\n", A);
}

value ::= INTEGER(A). {
	printf("%s\n", A);
}
value ::= FLOAT(A). {
	printf("%s\n", A);
}
value ::= QUOTED_STRING(A). {
	printf("%s\n", A);
}
value ::= KEYWORD(A). {
	printf("%s\n", A);
}

%parse_accept {
	printf("Parsing complete!\n");
}
%parse_failure {
	fprintf(stderr, "Parse failed.\n");
}
%stack_overflow {
	fprintf(stderr, "Parser stack overflow.\n");
}
