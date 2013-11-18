#ifndef DEBUG_HELPER_FUNCTION_H_INCLUDED
#define DEBUG_HELPER_FUNCTION_H_INCLUDED

#include "lexer.h"
extern struct token_sy token_history[120];
extern int idx;


void get_token_with_history();
void describe_token(struct token_sy token);
void print_action(char * const str, int * const vals);
void describe_token_history(struct token_sy const *token_history, int st, int en);
void print_error(const char x[]);
void print_verbose(const char x[]);

#endif // DEBUG_HELPER_FUNCTION_H_INCLUDED
