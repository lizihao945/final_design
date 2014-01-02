#include "dag.h"

int entry_bb, exit_bb;
int bb_map[1024][1024];
int in_bb[1024];
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
	t_quad_arg err;
	for (i = 0; i < node_list_top; i++)
		if (node_list[i].dag_node_idx == dag_idx)
			return node_list[i].quad_arg;
	// dag not found in node list
	err.arg_code = ARG_IMMEDIATE;
	err.val.int_val = 123456789;
	return err;
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
	// need data analysis
	for (i = 0; i < node_list_top; i++)
		if (node_list[i].quad_arg.arg_code == ARG_SYMBOL) {
			new_quadruple[new_quadruple_top].op = QUAD_ASSIGN;
			new_quadruple[new_quadruple_top].arg1 = node_list[i].quad_arg;
			new_quadruple[new_quadruple_top].arg2 = gen_quad_arg(node_list[i].dag_node_idx);
			if (new_quadruple[new_quadruple_top].arg2.arg_code == ARG_SYMBOL
				&& (!strcmp(new_quadruple[new_quadruple_top].arg2.symbol_item->name,
									new_quadruple[new_quadruple_top].arg1.symbol_item->name)))
				continue;
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
	for (i = 1; i < in_bb[0]; i++)
		if (in_bb[i] == quad_idx)
			return;
	in_bb[in_bb[0]++] = quad_idx;
}

int find_bb_idx(int label_idx) {
	int i;
	for (i = 1; i < in_bb[0]; i++)
		if (quadruple[in_bb[i]].op == QUAD_LABEL && quadruple[in_bb[i]].arg1.val.int_val == label_idx)
			return i;
	// bb including the label inst. not found
	return -1;
}

void devide_bb() {
	int quad_idx, i;
	in_bb[0] = 1;
	for (quad_idx = 0; quad_idx < quadruple_top; quad_idx++)
		switch (quadruple[quad_idx].op) {
			case QUAD_JMP:
			case QUAD_JMPF:
				add_in_quad(quad_idx + 1);
				break;
			case QUAD_LABEL:
				add_in_quad(quad_idx);
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
	// last bb is invalid
	add_in_quad(quadruple_top);
	in_bb[0]--;
	// generate two virtual basic blocks
	entry_bb = in_bb[0];
	exit_bb = in_bb[0] + 1;
	memset(bb_map, 0, 1024 * 1024 * sizeof(int));
	bb_map[entry_bb][1] = 1;
	for (i = 2; i <= in_bb[0]; i++)
		if (quadruple[in_bb[i] - 1].op == QUAD_JMP)
			bb_map[i - 1][find_bb_idx(quadruple[in_bb[i] - 1].arg1.val.int_val)] = 1;
		else if (quadruple[in_bb[i] - 1].op == QUAD_JMPF) {
			bb_map[i - 1][find_bb_idx(quadruple[in_bb[i] - 1].arg1.val.int_val)] = 1;
			bb_map[i - 1][i] = 1;
		} else if (quadruple[in_bb[i] - 1].op == QUAD_PROCEND && !strcmp(quadruple[in_bb[i] - 1].arg1.symbol_item->name, "start"))
			bb_map[i - 1][in_bb[0] + 1] = 1;
		else // somewhere jumps to next quad
			bb_map[i - 1][i] = 1;
}

void gen_dag() {
	int i, j, k, flag;
	int left_idx, right_idx, dag_idx;
	devide_bb();
	new_quadruple_top = 0;
	for (i = 1; i < in_bb[0]; i++) {
		//////////////////////////////////////////////////////////////////////////
		// check if array or call in the bb
		flag = 0;
		for (j = in_bb[i]; j < in_bb[i + 1]; j++)
			if (quadruple[j].op == QUAD_SETARRAY || quadruple[j].op == QUAD_GETARRAY || quadruple[j].op == QUAD_CALL) {
				for (k = in_bb[i]; k < in_bb[i + 1]; k++)		
					new_quadruple[new_quadruple_top++] = quadruple[k];
				flag = 1;
				break;
			}
		//////////////////////////////////////////////////////////////////////////
		if (flag)
			continue;
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
				case QUAD_JMP:
					break;
				default:
					new_quadruple[new_quadruple_top++] = quadruple[j];
					break;
			}
		}
		printf("******** bb%d from %d to %d\n", i, in_bb[i], in_bb[i + 1] - 1);
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
		// jmp should be the last quadruple
		gen_quadruples();
		if (quadruple[in_bb[i + 1] - 1].op == QUAD_JMP)
			new_quadruple[new_quadruple_top++] = quadruple[in_bb[i + 1] - 1];
		//////////////////////////////////////////////////////////////////////////
		printf("dag_idx_queue ****\n");
		for (j = 0; j < dag_idx_queue_top; j++)
			printf("%d, ", dag_idx_queue[j]);
		printf("\n");
		printf("******** bb%d ********\n\n", i);
	}
	quadruple_top = new_quadruple_top;
	for (j = 0; j < new_quadruple_top; j++)
		quadruple[j] = new_quadruple[j];
}
