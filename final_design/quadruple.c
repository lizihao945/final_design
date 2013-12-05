#include "quadruple.h"

struct quadruple_st quadruple[MAX_QUAD_NUM];
int label[MAX_QUAD_NUM];
int quadruple_top, label_top;

t_quad_arg  quadruple_add(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_ADD;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_sub(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_SUB;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_mult(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_MULT;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_div(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_DIV;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_les(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_LES;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_leq(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_LEQ;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_gtr(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_GTR;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}
t_quad_arg  quadruple_geq(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_GEQ;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	printf("\n");
	return rt;
}

t_quad_arg  quadruple_eql(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_EQL;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	return rt;
}

t_quad_arg  quadruple_neql(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg  rt;
	quadruple[quadruple_top].op = QUAD_NEQL;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	return rt;
}

/**
 * label is set to next quadruple
 */
int quadruple_lable() {
	quadruple[quadruple_top].op = QUAD_LABEL;
	quadruple[quadruple_top].arg1.arg_code = ARG_LABEL;
	quadruple[quadruple_top].arg1.val.int_val = label_top;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	label[label_top] = quadruple_top++;
	return label_top++;
}

/**
 * jump to label arg1, arg2 left for condition value
 */
int quadruple_jmpf(t_quad_arg arg2) {
	quadruple[quadruple_top].op = QUAD_JMPF;
	quadruple[quadruple_top].arg1.arg_code = ARG_LABEL;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple[quadruple_top].result.arg_code = 0;
	return quadruple_top++;
}

int quadruple_jmp() {
	quadruple[quadruple_top].op = QUAD_JMP;
	quadruple[quadruple_top].arg1.arg_code = ARG_LABEL;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	return quadruple_top++;
}

void quadruple_assign(t_quad_arg  arg1, t_quad_arg  arg2) {
	quadruple[quadruple_top].op = QUAD_ASSIGN;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}

t_quad_arg quadruple_getarray(t_quad_arg  arg1, t_quad_arg  arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_GETARRAY;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	return rt;
}

void quadruple_setarray(t_quad_arg  arg1, t_quad_arg  arg2, t_quad_arg  result) {
	quadruple[quadruple_top].op = QUAD_SETARRAY;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple[quadruple_top].result = result;
	quadruple_top++;
}

// call arg1() with ct arguments and put return value in result
t_quad_arg quadruple_call(t_quad_arg  arg1, int ct) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_CALL;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = ARG_IMMEDIATE;
	quadruple[quadruple_top].arg2.val.int_val = ct;
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	quadruple[quadruple_top].result = rt;
	quadruple_top++;
	return rt;
}

void quadruple_write(t_quad_arg arg1) {
	quadruple[quadruple_top].op = QUAD_WRITE;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}

void quadruple_read(t_quad_arg arg1) {
	quadruple[quadruple_top].op = QUAD_READ;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}

void quadruple_paramref(t_quad_arg arg1) {
	quadruple[quadruple_top].op = QUAD_PARAMREF;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}

void quadruple_paramval(t_quad_arg arg1) {
	quadruple[quadruple_top].op = QUAD_PARAMVAL;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}

void quadruple_procmark(t_quad_arg arg1) {
	quadruple[quadruple_top].op = QUAD_PROCMARK;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}

void quadruple_procend(t_quad_arg arg1) {
	quadruple[quadruple_top].op = QUAD_PROCEND;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2.arg_code = 0;
	quadruple[quadruple_top].result.arg_code = 0;
	quadruple_top++;
}
