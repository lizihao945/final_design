#include "dag.h"

int in_bb[1024];
int in_bb_top;
struct node_st node_list[1024];
int node_list_top;
struct dag_st dag[1024];
int dag_top;

// find leaf of arg_code with int_val
int find_or_create_node(struct quad_arg_st arg) {
	int i, idx;
	idx = -1;
	for (i = 0; i < node_list_top; i++)
		if (node_list[i].quad_arg.arg_code == arg.arg_code) {
			if (node_list[i].quad_arg.arg_code == ARG_IMMEDIATE && node_list[i].quad_arg.val.int_val == arg.val.int_val)
				return node_list[i].dag_node_idx;
			if (node_list[i].quad_arg.arg_code == ARG_SYMBOL &&
				node_list[i].quad_arg.symbol_item->depth == arg.symbol_item->depth &&
				!strcmp(node_list[i].quad_arg.symbol_item->name, arg.symbol_item->name))
				return node_list[i].dag_node_idx;
			if (node_list[i].quad_arg.arg_code == ARG_TEMP_IDX && node_list[i].quad_arg.val.int_val == arg.val.int_val)
				return node_list[i].dag_node_idx;
		}
	node_list[node_list_top].quad_arg = arg;
	dag[dag_top].op = 0; // leaf
	node_list[node_list_top].dag_node_idx = dag_top;
	node_list_top++;
	return dag_top++;
}

int find_or_create_dag(int op, int left_idx, int right_idx) {
	int i;
	for (i = 0; i < dag_top; i++)
		if (dag[i].op == op && dag[i].left_idx == left_idx && dag[i].right_idx == right_idx)
			return i;
	dag[dag_top].op = op;
	dag[dag_top].left_idx = left_idx;
	dag[dag_top].right_idx = right_idx;
	return dag_top++;
}

void opt(struct quad_arg_st arg, int dag_idx) {
	int i;
	for (i = 0; i < node_list_top; i++)
		if (node_list[i].quad_arg.arg_code == arg.arg_code) {
			if (node_list[i].quad_arg.arg_code == ARG_IMMEDIATE && node_list[i].quad_arg.val.int_val == arg.val.int_val) {
				node_list[i].dag_node_idx = dag_idx;
				return;
			}
			if (node_list[i].quad_arg.arg_code == ARG_SYMBOL &&
				node_list[i].quad_arg.symbol_item->depth == arg.symbol_item->depth &&
				!strcmp(node_list[i].quad_arg.symbol_item->name, arg.symbol_item->name)) {
					node_list[i].dag_node_idx = dag_idx;
					return;
			}
			if (node_list[i].quad_arg.arg_code == ARG_TEMP_IDX && node_list[i].quad_arg.val.int_val == arg.val.int_val) {
				node_list[i].dag_node_idx = dag_idx;
				return;
			}
		}
	node_list[node_list_top].quad_arg = arg;
	node_list[node_list_top].dag_node_idx = dag_idx;
	node_list_top++;
}

void add_in_quad(int quad_idx) {
	in_bb[in_bb_top++] = quad_idx;
}

void devide_bb() {
	int quad_idx;
	in_bb_top = 0;
	add_in_quad(0);
	for (quad_idx = 1; quad_idx < quadruple_top; quad_idx++)
		switch (quadruple[quad_idx].op) {
			case QUAD_JMP:
			case QUAD_JMPF:
				add_in_quad(quad_idx + 1);
				break;
			case QUAD_LABEL:
				add_in_quad(quad_idx + 1);
			default:
				break;
		}
	add_in_quad(quadruple_top);
}

void gen_dag() {
	int i, j;
	int left_idx, right_idx, dag_idx;
	node_list_top = 0;
	devide_bb();
	for (i = 0; i < in_bb_top; i++)
		for (j = in_bb[i]; j < in_bb[i + 1]; j++) {
			switch (quadruple[j].op) {
				case QUAD_ADD:
				case QUAD_SUB:
				case QUAD_MULT:
				case QUAD_DIV:
					// operand index
					left_idx = find_or_create_node(quadruple[j].arg1); // step 3
					right_idx = find_or_create_node(quadruple[j].arg2); // step 3
					dag_idx= find_or_create_dag(quadruple[j].op, left_idx, right_idx); // step 4
					opt(quadruple[j].result, dag_idx); // step 5
					break;
				default:
					break;
			}
		}
	for (i = 0; i < dag_top; i++)
		if (dag[i].op)
			printf("%d: %s %d %d\n", i, map_quad_string[dag[i].op], dag[i].left_idx, dag[i].right_idx);
		else
			printf("%d: leaf\n", i);
	for (i = 0; i < node_list_top; i++) {
		describe_quad_arg(node_list[i].quad_arg);
		printf(":\t%d\n", node_list[i].dag_node_idx);
	}
}
