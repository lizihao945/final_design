#ifndef DAG_H
#define DAG_H

#include <stdlib.h>
#include <string.h>
#include "quadruple.h"
#include "debug_helper_function.h"

void gen_dag();
void print_basic_blocks();

struct basic_block_st {
	int first_quad_idx, last_quad_idx;
};

struct dag_st {
	// '+', '0', '*,' '/' for interior node
	// 0 for leaf
	int op;
	// dag node index
	int left_idx, right_idx;
};

#endif