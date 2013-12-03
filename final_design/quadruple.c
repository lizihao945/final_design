#include "quadruple.h"

struct quadruple_st quadruple;

int quadruple_add(int arg1, int arg2) {
	quadruple.op = QUAD_ADD;
	quadruple.arg1 = arg1;
	quadruple.arg2 = arg2;
	printf("ADD\t%s\t%s\n", symbol_table[arg1].name, symbol_table[arg2].name);
	return push_item(TYPE_TEMP, TYPE_INTEGER, "t", 0);
}

int quadruple_mult(int arg1, int arg2) {
	quadruple.op = QUAD_MULT;
	quadruple.arg1 = arg1;
	quadruple.arg2 = arg2;
	printf("MULT\t%s\t%s\n", symbol_table[arg1].name, symbol_table[arg2].name);
	return push_item(TYPE_TEMP, TYPE_INTEGER, "t", 0);
}
