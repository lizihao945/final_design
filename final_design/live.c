#include "live.h"

struct var_list_st use_data[1024], def_data[1024], in_data[1024], out_data[1024];

void print_cfg() {
	int i, j;
	// print the cfg
	for (i = 1; i < in_bb[0]; i++)
		if (bb_map[in_bb[0]][i]) {
			printf("entry->%d\n", i);
			break;
		}
		for (i = 1; i < in_bb[0]; i++)
			for (j = 1; j < in_bb[0]; j++)
				if (bb_map[i][j])
					printf("%d->%d\n", i, j);
		for (i = 1; i < in_bb[0]; i++)
			if (bb_map[i][in_bb[0]+ 1])
				printf("%d->exit\n", i);
}

void initialize() {
	int i;
	// value of index 0 indicates top index
	for (i = 1; i < in_bb[0]; i++) {
		in_data[i].count = 0;
		out_data[i].count = 0;
		use_data[i].count = 0;
		def_data[i].count = 0;
	}
}

int add_sub(struct var_list_st *dest, struct var_list_st out, struct var_list_st def) {
	int i, j, def_idx, tmp, flag, flag1;
	tmp = dest->count;
	for (i = 0; i < out.count; i++) {
		flag = 0;
		// check if out.val[i] is not present in def.val
		for (def_idx = 0; def_idx < def.count; def_idx++)
			if (!strcmp(def.val[def_idx].symbol_item->name, out.val[i].symbol_item->name)) {
				flag = 1;
				break;
			}
		if (!flag) {
			// not repetition
			flag1 = 0;
			for (j = 0; j < dest->count; j++)
				if (!strcmp(dest->val[j].symbol_item->name, out.val[i].symbol_item->name)) {
					flag1 = 1;
					break;
				}
				if (flag1)
					continue;
			// add new ones
			dest->val[(dest->count)++] = out.val[i];
		}
	}
	return dest->count == tmp ? 0 : 1;
}

int add_all(struct var_list_st *dest, struct var_list_st source) {
	int i, j, tmp, flag;
	tmp = dest->count;
	for (i = 0; i < source.count; i++) {
		// not repetition
		flag = 0;
		for (j = 0; j < dest->count; j++)
			if (!strcmp(dest->val[j].symbol_item->name, source.val[i].symbol_item->name)) {
				flag = 1;
				break;
			}
		if (flag)
			continue;
		// add new ones
		dest->val[(dest->count)++] = source.val[i];
	}
	return dest->count == tmp ? 0 : 1;
}

void add_use(int bb_idx, struct quad_arg_st source) {
	int i;
	// nothing to do if def before use
	for (i = 0; i < def_data[bb_idx].count; i++)
		if (!strcmp(def_data[bb_idx].val[i].symbol_item->name, source.symbol_item->name))
			return;
	// not repetition
	for (i = 0; i < use_data[bb_idx].count; i++)
		if (!strcmp(use_data[bb_idx].val[i].symbol_item->name, source.symbol_item->name))
			return;
	 use_data[bb_idx].val[use_data[bb_idx].count++] = source;
}

void add_def(int bb_idx, struct quad_arg_st source) {
	int i;
	// nothing to do if use before def
	for (i = 0; i < use_data[bb_idx].count; i++)
		if (!strcmp(use_data[bb_idx].val[i].symbol_item->name, source.symbol_item->name))
			return;
	// not repetition
	for (i = 0; i < def_data[bb_idx].count; i++)
		if (!strcmp(def_data[bb_idx].val[i].symbol_item->name, source.symbol_item->name))
			return;
	def_data[bb_idx].val[def_data[bb_idx].count++] = source;
}

void gen_use_and_def() {
	int bb_idx, quad_idx;
	for (bb_idx = 1; bb_idx < in_bb[0]; bb_idx++) {
		for (quad_idx = in_bb[bb_idx]; quad_idx < in_bb[bb_idx + 1]; quad_idx++)
			switch (quadruple[quad_idx].op){
				case QUAD_LES:
				case QUAD_LEQ:
				case QUAD_GTR:
				case QUAD_GEQ:
				case QUAD_EQL:
				case QUAD_NEQL:
				case QUAD_ADD:
				case QUAD_SUB:
				case QUAD_MULT:
				case QUAD_DIV:
					if (quadruple[quad_idx].arg1.arg_code == ARG_SYMBOL)
						add_use(bb_idx, quadruple[quad_idx].arg1);
					if (quadruple[quad_idx].arg2.arg_code == ARG_SYMBOL)
						add_use(bb_idx, quadruple[quad_idx].arg2);
					break;
				case QUAD_ASSIGN:
					if (quadruple[quad_idx].arg1.arg_code == ARG_SYMBOL)
						add_def(bb_idx, quadruple[quad_idx].arg1);
					if (quadruple[quad_idx].arg2.arg_code == ARG_SYMBOL)
						add_use(bb_idx, quadruple[quad_idx].arg2);
					break;
		}
	}
}

void live_var_analysis() {
	int i, j, modified;
	int bb_idx, quad_idx;
	devide_bb();
	print_cfg();
	initialize();
	gen_use_and_def();
	//////////////////////////////////////////////////////////////////////////
	// analysis
	modified = 1;
	while (modified) {
		modified = 0;
		for (i = 1; i < in_bb[0]; i++) {
			// out[B] = union in[P]
			for (j = 1; j < in_bb[0]; j++)
				if (bb_map[i][j]) // inspect successors
					add_all(&(out_data[i]), in_data[j]);
			// in[B] = use[B] union (out[B] - def[B])
			modified += add_all(&(in_data[i]), use_data[i]);
			modified += add_sub(&(in_data[i]), out_data[i], def_data[i]);
			
		}
	}
	for (i = 1; i < in_bb[0]; i++)
		printf("******** bb%d from %d to %d\n", i, in_bb[i], in_bb[i + 1] - 1);
	//////////////////////////////////////////////////////////////////////////
	// print use def in out
	for (bb_idx = 1; bb_idx < in_bb[0]; bb_idx++) {
		if (use_data[bb_idx].count) {
			printf("use[%d]:\t\t", bb_idx);
			for (i = 0; i < use_data[bb_idx].count; i++)
				describe_quad_arg(use_data[bb_idx].val[i]);
			printf("\n");
		}
		if (def_data[bb_idx].count) {
			printf("def[%d]:\t\t", bb_idx);
			for (i = 0; i < def_data[bb_idx].count; i++)
				describe_quad_arg(def_data[bb_idx].val[i]);
			printf("\n");
		}
		if (in_data[bb_idx].count) {
			printf("in[%d]:\t\t", bb_idx);
			for (i = 0; i < in_data[bb_idx].count; i++)
				describe_quad_arg(in_data[bb_idx].val[i]);
			printf("\n");
		}
		if (out_data[bb_idx].count) {
			printf("out[%d]:\t\t", bb_idx);
			for (i = 0; i < out_data[bb_idx].count; i++)
				describe_quad_arg(out_data[bb_idx].val[i]);
			printf("\n");
		}
	}
}
