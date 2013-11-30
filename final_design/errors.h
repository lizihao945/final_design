#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include "debug_helper_function.h"
#define INVALID_CHAR 11
#define INVALID_ID 12
#define UNACC_CHAR 13
#define QMARK_MISSED 14
#define DQMARK_MISSED 15

#define TYPE_MISSMATCH_ON_ASSIGN 21
#define TYPE_NOT_DEFINED 22
#define TYPE_WRONG_ON_RETURN 23
#define ID_REDEFINED 24
#define ID_NOT_DEFINED 25
#define RPARENT_MISSED 26
#define RBRACK_MISSED 27
#define END_MISSED 28
#define ARG_TYPE_MISSMATCH 29
#define ARG_NUM_MISSMATCH 30
#define DIVIDED_BY_ZERO 31
#define ASSIGN_TO_CONST 32
#define RETURN_VAL_NOT_SET 33
#define UNACCEPTABLE 34

int eval_error(int error_type, const char *p);
#endif