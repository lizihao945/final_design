#include "quadruple.h"

struct quadruple_st quadruple[MAX_QUAD_NUM];
int quadruple_top;

t_quad_arg quadruple_add(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_ADD;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("ADD\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

struct quad_arg_st quadruple_sub(struct quad_arg_st arg1, struct quad_arg_st arg2) {
	struct quad_arg_st rt;
	quadruple[quadruple_top].op = QUAD_SUB;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("SUB\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

struct quad_arg_st quadruple_mult(struct quad_arg_st arg1, struct quad_arg_st arg2) {
	struct quad_arg_st rt;
	quadruple[quadruple_top].op = QUAD_MULT;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("MULT\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

struct quad_arg_st quadruple_div(struct quad_arg_st arg1, struct quad_arg_st arg2) {
	struct quad_arg_st rt;
	quadruple[quadruple_top].op = QUAD_DIV;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("DIV\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}
