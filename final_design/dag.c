#include "dag.h"

int in_bb[1024];
int in_bb_top;
struct node_st node_list[1024];
int node_list_top;
struct dag_st dag[1024];
int dag_top;
int in_queue[1024];
int dag_idx_queue[1024];
int dag_idx_queue_top;


int no_father(int dag_idx) {
	int i;
	for (i = 0; i < dag_top; i++)
		if (dag[i].op && !in_queue[i])
			if (dag[i].left_idx == dag_idx || dag[i].right_idx == dag_idx)
				return 0;
	return 1;
}

void add_queue(int dag_idx) {
	in_queue[dag_idx] = 1;
	dag_idx_queue[dag_idx_queue_top++] = dag_idx;
}

t_quad_arg gen_quad_arg(int dag_idx) {
	int i;
	for (i = 0; i < node_list_top; i++)
		if (node_list[i].dag_node_idx == dag_idx)
			return node_list[i].quad_arg;
}

void gen_quadruples() {
	int i, tmp, modified;
	memset(in_queue, 0, 1024 * sizeof(int));
	modified = 1;
	while (modified) {
		modified = 0;
		for (i = 0; i < dag_top; i++)
			if (dag[i].op && !in_queue[i] && no_father(i)) {
				modified = 1;
				add_queue(i);
				tmp = dag[i].left_idx;
				while (dag[tmp].op && !in_queue[tmp] && no_father(tmp)) {
					add_queue(tmp);
					tmp = dag[tmp].left_idx;
				}
			}
	}
	for (i = dag_idx_queue_top - 1; i >= 0; i--) {
		new_quadruple[new_quadruple_top].op = dag[dag_idx_queue[i]].op;
		new_quadruple[new_quadruple_top].arg1 = gen_quad_arg(dag[dag_idx_queue[i]].left_idx);
		new_quadruple[new_quadruple_top].arg2 = gen_quad_arg(dag[dag_idx_queue[i]].right_idx);
		new_quadruple[new_quadruple_top].result = gen_quad_arg(dag_idx_queue[i]);
		new_quadruple_top++;
	}
}

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
	int i;
	for (i = 0; i < in_bb_top; i++)
		if (in_bb[i] == quad_idx)
			return;
	in_bb[in_bb_top++] = quad_idx;
}

void devide_bb() {
	int quad_idx;
	in_bb_top = 0;
	for (quad_idx = 0; quad_idx < quadruple_top; quad_idx++)
		switch (quadruple[quad_idx].op) {
			case QUAD_JMP:
			case QUAD_JMPF:
				add_in_quad(quad_idx + 1);
				break;
			case QUAD_LABEL:
				add_in_quad(quad_idx + 1);
				break;
			case QUAD_READ:
			case QUAD_PROCMARK:
			case QUAD_PROCEND:
				add_in_quad(quad_idx);
				add_in_quad(quad_idx + 1);
				break;
			default:
				break;
		}
	add_in_quad(quadruple_top);
}

void gen_dag() {
	int i, j;
	int left_idx, right_idx, dag_idx;
	devide_bb();
	new_quadruple_top = 0;
	for (i = 0; i < in_bb_top; i++) {
		node_list_top = 0;
		dag_top = 0;
		dag_idx_queue_top = 0;
		for (j = in_bb[i]; j < in_bb[i + 1]; j++) {
			switch (quadruple[j].op) {
				case QUAD_ASSIGN: // left := right
					right_idx = find_or_create_node(quadruple[j].arg2);
					node_list[node_list_top].dag_node_idx = right_idx;
					node_list[node_list_top].quad_arg = quadruple[j].arg1;
					node_list_top++;
					break;
				case QUAD_ADD:
				case QUAD_SUB:
				case QUAD_MULT:
				case QUAD_DIV:
				case QUAD_WRITE:
					// operand index of dag(not node list!)
					left_idx = find_or_create_node(quadruple[j].arg1); // step 3
					right_idx = find_or_create_node(quadruple[j].arg2); // step 3
					dag_idx= find_or_create_dag(quadruple[j].op, left_idx, right_idx); // step 4
					opt(quadruple[j].result, dag_idx); // step 5
					break;
				default:
					new_quadruple[new_quadruple_top++] = quadruple[j];
					break;
			}
		}
		printf("******** bb%d ********\n", i);
		printf("dag ****\n");
		for (j = 0; j < dag_top; j++)
			if (dag[j].op)
				printf("%d: %s %d %d\n", j, map_quad_string[dag[j].op], dag[j].left_idx, dag[j].right_idx);
			else
				printf("%d: leaf\n", j);
		printf("node list ****\n");
		for (j = 0; j < node_list_top; j++) {
			describe_quad_arg(node_list[j].quad_arg);
			printf(":\t%d\n", node_list[j].dag_node_idx);
		}
		// generate quadruples from dag
		gen_quadruples();
		printf("dag_idx_queue ****\n");
		for (j = 0; j < dag_idx_queue_top; j++)
			printf("%d, ", dag_idx_queue[j]);
		printf("\n");
		printf("******** bb%d ********\n\n", i);
		printf("generated quadruples ****\n");
		quadruple_top = new_quadruple_top;
		for (j = 0; j < new_quadruple_top; j++) {
			quadruple[j] = new_quadruple[j];
			printf("\t%s\t", map_quad_string[new_quadruple[j].op]);
			describe_quad_arg(new_quadruple[j].arg1);
			describe_quad_arg(new_quadruple[j].arg2);
			describe_quad_arg(new_quadruple[j].result);
			printf("\n");
		}
	}
}
