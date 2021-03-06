#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include "debug_helper_function.h"
#define ERR_INVALID_CHAR 11
#define ERR_INVALID_ID 12
#define ERR_UNACC_CHAR 13
#define ERR_QMARK_MISSED 14
#define ERR_DQMARK_MISSED 15

#define ERR_TYPE_MISSMATCH_ON_ASSIGN 21
#define ERR_TYPE_NOT_DEFINED 22
#define ERR_TYPE_WRONG_ON_RETURN 23
#define ERR_ID_REDEFINED 24
#define ERR_ID_NOT_DEFINED 25
#define ERR_RPARENT_MISSED 26
#define ERR_LBRACK_MISSED 271
#define ERR_RBRACK_MISSED 27
#define ERR_END_MISSED 28
#define ERR_ARG_TYPE_MISSMATCH 29
#define ERR_ARG_NUM_MISSMATCH 30
#define ERR_DIVIDED_BY_ZERO 31
#define ERR_ASSIGN_TO_CONST 32
#define ERR_RETURN_VAL_NOT_SET 33
#define ERR_UNACCEPTABLE 34
#define ERR_INVALID_ARRAY_IDX 35
#define ERR_SEMICN_MISSED 36
#define ERR_COLON_MISSED 37
#define	 ERR_PARAMETER_MISSED 38
#define ERR_STACK_OVERFLOW 39

void judge_type(struct quad_arg_st p, struct quad_arg_st q);
int eval_error(int error_type, const char *p);
#endif