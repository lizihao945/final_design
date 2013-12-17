#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include <stdio.h>
#include "symbol_table.h"
#include "debug_helper_function.h"
#include "env.h"
#include "gen_asm.h"

#define ARG_LABEL 904
#define ARG_IMMEDIATE 905
#define ARG_SYMBOL 906
#define ARG_TEMP_IDX 907
#define ARG_STRING 908

#define QUAD_PROCEND 513
#define QUAD_PROCMARK 514
#define QUAD_LEQ 515 // <=
#define QUAD_GEQ 516 // >=
#define QUAD_ADD 517
#define QUAD_SUB 518
#define QUAD_MULT 519
#define QUAD_DIV 520
#define QUAD_EQL 521 // =
#define QUAD_NEQL 522 // <>
#define QUAD_GTR 523 // >
#define QUAD_LES 524 // <
#define QUAD_DEF 525
#define QUAD_PARAMVAL 526 //
#define QUAD_PARAMREF 527 // var
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
#define QUAD_WRITE 539
#define QUAD_READ 540

typedef struct quad_arg_st {
	int arg_code;
	union {
		int int_val;
		char str_val[MAX_NAME];
	} val;
	struct symbol_item_st *symbol_item;
} t_quad_arg;

typedef struct linked_quad_args_st {
	t_quad_arg val;
	struct linked_quad_args_st *next;
} t_linked_quad_args;

typedef struct quadruple_st {
	int op;
	t_quad_arg arg1, arg2, result;
} t_quadruple;

extern t_quadruple quadruple[MAX_QUAD_NUM];
extern int label[];
extern int quadruple_top, label_top;
t_quad_arg quadruple_add(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_sub(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_mult(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_div(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_les(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_leq(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_gtr(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_geq(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_eql(t_quad_arg arg1, t_quad_arg arg2);
t_quad_arg quadruple_neql(t_quad_arg arg1, t_quad_arg arg2);
void quadruple_assign(t_quad_arg  arg1, t_quad_arg  arg2);
int quadruple_lable();
int quadruple_jmpf(t_quad_arg arg2);
int quadruple_jmp();
t_quad_arg quadruple_getarray(t_quad_arg  arg1, t_quad_arg  arg2);
void quadruple_setarray(t_quad_arg  arg1, t_quad_arg  arg2, t_quad_arg  result);
t_quad_arg quadruple_call(t_quad_arg  arg1, int ct);
void quadruple_write(t_quad_arg arg1);
void quadruple_paramref(t_quad_arg arg1);
void quadruple_paramval(t_quad_arg arg1);
int quadruple_procmark(t_quad_arg arg1, int count);
void quadruple_procend(t_quad_arg arg1);
#endif