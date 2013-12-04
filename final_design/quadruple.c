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

t_quad_arg quadruple_sub(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
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

t_quad_arg quadruple_mult(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
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

t_quad_arg quadruple_div(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
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

t_quad_arg quadruple_les(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_LES;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("LES<\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

t_quad_arg quadruple_leq(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_LES;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("LEQ<=\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

t_quad_arg quadruple_gtr(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_GTR;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("GTR>\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}
t_quad_arg quadruple_geq(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_LES;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("GEQ>=\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

t_quad_arg quadruple_eql(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_EQL;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("EQL=\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}

t_quad_arg quadruple_neql(t_quad_arg arg1, t_quad_arg arg2) {
	t_quad_arg rt;
	quadruple[quadruple_top].op = QUAD_NEQL;
	quadruple[quadruple_top].arg1 = arg1;
	quadruple[quadruple_top].arg2 = arg2;
	quadruple_top++;
	printf("NEQL<>\t");
	describe_quad_arg(arg1);
	describe_quad_arg(arg2);
	rt.arg_code = ARG_TEMP_IDX;
	rt.val.idx = push_temp();
	describe_quad_arg(rt);
	printf("\n");
	return rt;
}
