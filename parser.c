/*
	Zihao Li
	11061111
	Nov. 1st, 2013
*/

#include"lexer.h"
#include"parser.h"
FILE *in;
struct token_sy token;

void print_error(const char x[]) {
    printf("Error\t\t%s!\n", x);
}

void print_verbose(const char x[]) {
    printf("Verbose\t\t%s.\n", x);
}

void parse_factor() {
    switch (token.sy) {
    case LPARENT:
        get_token(in, &token);
        parse_expression();
        if (token.sy != RPARENT) {
            print_error("no RPARSY found after LPARSY");
        }
        get_token(in, &token);
        break;
    default:
        parse_var();
        break;
    }
    print_verbose("a factor");
}

void parse_var() {
    if (token.sy != IDEN||
            strcmp(token.val.strVal, "i")) {
        print_error("not proper VAR found");
    }
    get_token(in, &token);
    if (token.sy == LBRACK) {
        get_token(in, &token);
        parse_expression();
        if (token.sy != RBRACK) {
            print_error("no RBARSY found after LBRASY");
        }
        get_token(in, &token);
    }
}

void parse_term() {
    parse_factor();
    while (token.sy == MULT) {
        get_token(in, &token);
        parse_factor();
    }
    print_verbose("a term");
}

void parse_expression() {
    parse_term();
    while (token.sy == PLUS) {
        get_token(in, &token);
        parse_term();
    }
    print_verbose("a expression");
}

void parse_if_statement() {
    if (token.sy != IFTK)
        return;
    get_token(in, &token);
    parse_expression();
    if (token.sy != THENTK) {
        print_error("no THEN found after IF");
    }
    get_token(in, &token);
    parse_statement();
    get_token(in, &token);
    parse_else();
    print_verbose("a IF statement");
}

void parse_else() {
    if (token.sy != ELSETK)
        return;
    get_token(in, &token);
    parse_statement();
    print_verbose("a IF statement with ELSE");
}

void parse_statement() {
    if (token.sy == IFTK)
        parse_if_statement();
    else
        parse_var();
    print_verbose("a statement");
}
