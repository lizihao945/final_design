#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

void parse_else();

void parse_term();
void parse_terms();
void parse_factor();
void parse_var();
void parse_expression();
void parse_expressions();

void parse_str();

void parse_write();
void parse_optwrite();
void parse_read();
void parse_optread();

void parse_if_statement();
void parse_statement();

void print_error(const char x[]);
void print_verbose(const char x[]);
#endif // PARSER_H_INCLUDED
