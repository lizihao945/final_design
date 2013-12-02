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

void test_var_part();
void test_var_def();
void test_type();
void test_primitive_type();

void test_const_def();
void test_cond();
void test_argument();
void test_expression();

void test_statement();
void test_if_statement();
void test_while_statement();
void test_for_statement();
void test_compound_statement();
void test_assign_statement();

#endif // DEBUG_HELPER_FUNCTION_H_INCLUDED
