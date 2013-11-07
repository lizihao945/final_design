#ifndef PARSER_FIRST_H_INCLUDED
#define PARSER_FIRST_H_INCLUDED

void parse_else();
void parse_expression();
void parse_if_statement();
void parse_statement();
void parse_term();
void parse_factor();
void parse_var();

void print_error(const char x[]);
void print_verbose(const char x[]);
#endif // PARSER_FIRST_H_INCLUDED
