#include"lexer.h"
#include"parser.h"
#include "debug_helper_function.h"
void parse_if_statement() {
    if (token.sy != IFTK)
        return;
    get_token_with_history();
    parse_expression();
    if (token.sy != THENTK) {
        print_error("no THEN found after IF");
    }
    get_token_with_history();
    parse_statement();
    get_token_with_history();
    parse_else();
    print_verbose("a IF statement");
}

void parse_else() {
    if (token.sy != ELSETK)
        return;
    get_token_with_history();
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

void parse_term() {
    int i = idx;
    parse_factor();
    parse_terms();
    describe_token_history(token_history, i, idx);
    print_verbose("a term");
}

void parse_terms() {
    int i = idx;
    if (token.sy == MULT) {
        get_token_with_history();
        parse_factor();
        parse_terms();
        describe_token_history(token_history, i, idx);
        print_verbose("a terms(not empty)");
    } else {
        print_verbose("a terms(empty)");
    }
}

void parse_expression() {
    int i = idx;
    parse_term();
    parse_expressions();
    describe_token_history(token_history, i, idx);
    print_verbose("a expression");
}

void parse_expressions() {
    int i = idx;
    if (token.sy == PLUS) {
        get_token_with_history();
        parse_term();
        parse_expressions();
        describe_token_history(token_history, i, idx);
        print_verbose("a expressions(not empty)");
    } else {
        print_verbose("a expressions(empty)");
    }
}

void parse_var() {
    int i = idx;
    if (token.sy != IDEN) {
        print_error("should be IDEN here");
    }
    get_token_with_history();
    if (token.sy == LBRACK) {
        get_token_with_history();
        parse_expression();
        if (token.sy != RBRACK) {
            print_error("no RBARSY found after LBRASY");
        }
        get_token_with_history();
    } else {
        // a IDEN taken as a var()
    }
    describe_token_history(token_history, i, idx);
    print_verbose("a var");
}

void parse_factor()  {
    int i = idx;
    switch (token.sy) {
    case IDEN:
        parse_var();
        break;
    case INTCON:
        get_token_with_history();
        break;
    case LPARENT:
        get_token_with_history();
        parse_expression();
        if (token.sy != RPARENT) {
            print_error("no RPARSY found after LPARSY");
        }
        get_token_with_history();
        break;
    default:
        break;
    }
    describe_token_history(token_history, i, idx);
    print_verbose("a factor");
}

int main() {
//    char tmp[32], tmp2[32];
//    printf("Input your source file name:\n");
//    scanf("%s", tmp);
    init_map_sy_string();
    in = fopen("test_by_myself.txt", "r");
    get_token_with_history();
    parse_expression();
    fclose(in);
    return 0;
}
