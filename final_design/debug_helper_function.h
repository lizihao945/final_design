#ifndef DEBUG_HELPER_FUNCTION_H_INCLUDED
#define DEBUG_HELPER_FUNCTION_H_INCLUDED

#define MAX_TOKEN_NUM 1024

#include "lexer.h"
#include "parser.h"
#include "quadruple.h"
#include "env.h"
#include "gen_asm.h"

// global variables
extern int verbose_off;
extern int describe_token_off;
extern struct token_st token_history[];
extern int idx;
extern const char * map_quad_string[];
extern const char * map_type_string[];

// interfaces

void init_fake_symbol_table();

void print_symbol_table();
void init_map_type_string();

void describe_quad_arg(struct quad_arg_st arg);
void print_quadruples();
void init_map_quad_string();

void print_tokens(FILE *in);
void get_token_with_history();
void describe_token(struct token_st token);
void describe_token_history(int st, int en);
void print_error(const char x[]);
void print_verbose(const char x[]);

void test_program();
void test_const_part();
void test_const_def();

void test_var_part();
void test_var_def();

void test_procedure_part();
void test_procedure_head();
void test_function_part();
void test_function_head();
void test_parameter_list();
void test_parameter();

void test_cond();
void test_argument();
void test_expression();

void test_statement();
void test_if_statement();
void test_while_statement();
void test_for_statement();
void test_compound_statement();
void test_assign_statement();

void do_compile_job();
#endif // DEBUG_HELPER_FUNCTION_H_INCLUDED
