#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include "symbol_table.h"
#include "debug_helper_function.h"
#include <stdio.h>

#define MAX_QUAD_NUM 1024

#define ARG_IMMEDIATE 905
#define ARG_SYMBOL_IDX 906
#define ARG_TEMP_IDX 907

#define QUAD_ADD 517
#define QUAD_SUB 518
#define QUAD_MULT 519
#define QUAD_DIV 520
#define QUAD_EQL 521
#define QUAD_NEQL 522
#define QUAD_GTR 523
#define QUAD_LES 524
#define QUAD_DEF 525
#define QUAD_PARAMVAL 526
#define QUAD_PARAMREF 527
#define QUAD_ARGU 528
#define QUAD_CALL 529
#define QUAD_RET 530
#define QUAD_ASSIGN 531
#define QUAD_GETARRAY 532
#define QUAD_SETARRAY 533
#define QUAD_LABEL 534
#define QUAD_JMP 535
#define QUAD_JMPF 536
#define QUAD_CTOI 537
#define QUAD_ITOC 538

typedef struct quad_arg_st {
	int arg_code;
	union {
		int idx;
		int int_val;
	} val;
} t_quad_arg;

typedef struct linked_quad_args_st {
	struct quad_arg_st val;
	struct linked_quad_args_st *next;
} t_linked_quad_args;

typedef struct quadruple_st {
	int op, result;
	struct quad_arg_st arg1, arg2;
} t_quadruple;

extern t_quadruple quadruple[MAX_QUAD_NUM];
extern int quadruple_top;
struct quad_arg_st quadruple_add(struct quad_arg_st arg1, struct quad_arg_st arg2);
struct quad_arg_st quadruple_sub(struct quad_arg_st arg1, struct quad_arg_st arg2);
struct quad_arg_st quadruple_mult(struct quad_arg_st arg1, struct quad_arg_st arg2);
struct quad_arg_st quadruple_div(struct quad_arg_st arg1, struct quad_arg_st arg2);
#endif