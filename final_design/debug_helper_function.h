#ifndef DEBUG_HELPER_FUNCTION_H_INCLUDED
#define DEBUG_HELPER_FUNCTION_H_INCLUDED

#include "lexer.h"
#include "parser.h"
extern struct token_sy token_history[120];
extern int idx;

void print_tokens(FILE *in);
void get_token_with_history();
void describe_token(struct token_sy token);
void print_action(char * const str, int * const vals);
void describe_token_history(int st, int en);
void print_error(const char x[]);
void print_verbose(const char x[]);

void test_const();
void test_cond();
void test_if_statement();

#endif // DEBUG_HELPER_FUNCTION_H_INCLUDED
