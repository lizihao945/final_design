#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include"lexer.h"
#include "symbol_table.h"
#include "debug_helper_function.h"

void parse_id();

void parse_program();
void parse_sub_program();

void parse_const_part();
void parse_const_def();

void parse_var_part();
void parse_var_def();
void parse_type();
void parse_primitive_type();

void parse_procedure_part();
void parse_procedure_head();
void parse_function_part();
void parse_function_head();
void parse_parameter_list();
void parse_parameter();

void parse_cond();
void parse_expression();
void parse_optexpression();
void parse_term();
void parse_optterm();
void parse_factor();
void parse_var();
void parse_argument();
void parse_optargument();

void parse_str();

void parse_write();
void parse_optwrite();
void parse_read();
void parse_optread();

void parse_statement();
void parse_if_statement();
void parse_while_statement();
void parse_for_statement();
void parse_else();
void parse_compound_statement();
void parse_optcompound_statement();
void parse_assign_statement();

void print_error(const char x[]);
void print_verbose(const char x[]);
#endif // PARSER_H_INCLUDED
