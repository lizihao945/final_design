#include "gen_asm.h"
int quad_idx;
t_quadruple cur_procedure;

int flag_reg[8]; // reg used?
char * reg_name[8] = {"eax", "ecx", "edx", "esp", "ebp", "ebx", "esi", "edi"};
int allocation_top;
struct allocation_st allocation[1024];
// reg_queue
// head_idx indicates first in the queue when not empty
// tail_idx indicates next available place
int head_idx, tail_idx;
int reg_queue[1048575];

void prog_head();
void gen_jcc(int flag);
void describe_quad_arg_to_file(t_quad_arg arg);

void free_regs() {
	int i;
	for (i = 0; i < 6; i++)
		flag_reg[i] = 0;
}

int empty_reg_queue() {
	return head_idx < tail_idx ? 0 : 1;
}

void in_reg_queue(int reg_idx) {
	reg_queue[tail_idx++] = reg_idx;
}

int out_reg_queue() {
	if (!empty_reg_queue())
		return reg_queue[head_idx++];
	else
		return -1;
}

int same_arg(t_quad_arg arg1, t_quad_arg arg2) {
	if (arg1.arg_code != arg2.arg_code)
		return 0;
	if (arg1.arg_code == ARG_SYMBOL && !(strcmp(arg1.symbol_item->name, arg2.symbol_item->name)))
		return 1;
	if (arg1.arg_code == ARG_TEMP_IDX && arg1.val.int_val == arg2.val.int_val)
		return 1;
	return 0;
}

int allocated(t_quad_arg arg) {
	int i;
	for (i = 0; i < allocation_top; i++)
		if (allocation[i].arg.arg_code == arg.arg_code) {
			if(allocation[i].arg.arg_code == ARG_SYMBOL && !(strcmp(allocation[i].arg.symbol_item->name, arg.symbol_item->name)))
				return allocation[i].reg_idx;
			if(allocation[i].arg.arg_code == ARG_TEMP_IDX && allocation[i].arg.val.int_val == arg.val.int_val)
				return allocation[i].reg_idx;
		}
	return -1;
}

/**
 * the value placed in regs in this procedure should not be freed
 * until the address of arg is calculated
 */
void arg_memory_str(t_quad_arg arg, struct asm_arg_st *asm_arg) {
	char *tmp;
	char *s;
	int cur_depth, arg_depth;
	// s represents return string
	s = asm_arg->name;
	s[0] = '\0';
	switch (arg.arg_code) {
		case ARG_SYMBOL:
			// procedure
			if (arg.symbol_item->category_code == CATEGORY_PROCEDURE) {
				strcat(s, arg.symbol_item->name);
				break;
			}
			// function
			if (arg.symbol_item->category_code == CATEGORY_FUNCTION) {
				arg_depth = arg.symbol_item->depth;
				strcat(s, "dword ptr [ebp+");
				tmp = (char *) malloc(sizeof(char) * 256);
				// return address, display, return value address, params
				itoa((1 + arg_depth + 1) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				fprintf(out, "\tmov ebx, %s\n", s);
				strcpy(s, "dword ptr [ebx]");
				break;
			}
			// const
			if (arg.symbol_item->category_code == CATEGORY_CONST) {
				tmp = (char *) malloc(sizeof(char) * 256);
				itoa(arg.symbol_item->val.int_val, tmp, 10);
				strcat(s, tmp);
				break;
			}
			// parameter
			arg_depth = arg.symbol_item->depth;
			if (arg.symbol_item->category_code == CATEGORY_PARAMREF || arg.symbol_item->category_code == CATEGORY_PARAMVAL) {
				strcat(s, "dword ptr [ebp+");
				tmp = (char *) malloc(sizeof(char) * 256);
				// first parameter first
				// return address, display, return value address, params
				itoa(4 + (arg_depth - 1) * 4 + 4 + (cur_procedure.arg1.symbol_item->param_count - arg.symbol_item->param_idx + 1) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				if (arg.symbol_item->category_code == CATEGORY_PARAMREF) {					
					// allocate a free register to save address of the parameter
					fprintf(out, "\tmov ebx, %s\n", s);
					s[0] = '\0';
					strcat(s, "dword ptr [ebx]");
					break;
				}
				break;
			}
			// local
			// the depth of cur_procedure is less than the depth of its content
			cur_depth = cur_procedure.arg1.symbol_item->depth + 1;
			if (arg_depth < cur_depth) {
				// abp to arg_depth layer
				fprintf(out, "\tmov ebx, dword ptr [ebp+%d]\n", (cur_depth - arg_depth + 1) * 4);
				strcat(s, "dword ptr [ebx-");
				tmp = (char *) malloc(sizeof(char) * 256);
				itoa((arg.symbol_item->offset_byte) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				break;
			}
			strcat(s, "dword ptr [ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.symbol_item->offset_byte * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			break;
		case ARG_IMMEDIATE:
			itoa(arg.val.int_val, s, 10);
			break;
		case ARG_TEMP_IDX:
			strcat(s, "dword ptr [ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			// first temp is t0
			itoa((arg.val.int_val + 1 + cur_procedure.arg2.val.int_val) * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			break;
		case ARG_LABEL:
			strcat(s, "label");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.val.int_val, tmp, 10);
			strcat(s, tmp);
			break;
		case ARG_STRING:
			strcat(s, "string");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.val.int_val, tmp, 10);
			strcat(s, tmp);
			break;
	}
}

void no_cancel_writeback(int reg_idx) {
	struct asm_arg_st *arg;
	int i;
	arg = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg->name = (char *) malloc(sizeof(char) * 256);
	for (i = 0; i < allocation_top; i++)
		if (allocation[i].reg_idx == reg_idx) {
			arg_memory_str(allocation[i].arg, arg);
			fprintf(out, "\tmov %s, %s\n", arg->name, reg_name[allocation[i].reg_idx]);
		}
}

void cancel_allocation(int reg_idx) {
	struct asm_arg_st *arg;
	int i;
	if (reg_idx == -1)
		return;
	arg = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg->name = (char *) malloc(sizeof(char) * 256);
	for (i = 0; i < allocation_top; i++)
		if (allocation[i].reg_idx == reg_idx) {
			if (allocation[i].arg.arg_code == ARG_IMMEDIATE)
				continue;
			if (allocation[i].arg.arg_code == ARG_SYMBOL && allocation[i].arg.symbol_item->category_code == CATEGORY_CONST)
				continue;
			arg_memory_str(allocation[i].arg, arg);
			fprintf(out, "; write back ****\t");
			describe_quad_arg_to_file(allocation[i].arg);
			fprintf(out, "****\n");
			fprintf(out, "\tmov %s, %s\n", arg->name, reg_name[allocation[i].reg_idx]);
			// easy way to make this allocation unavailable
			allocation[i].reg_idx = -1;
			allocation[i].arg.arg_code = 0;
		}
}

int get_reg_idx_to_allocate(t_quad_arg arg) {
	int i, idx;
	// if allocated, return
	if ((idx = allocated(arg)) != -1)
		return idx;
	// if there is a empty register, use it
	for (i = REG_EAX; i <= REG_EDX; i++)
		if (!flag_reg[i]) {
			allocation[allocation_top].arg= arg;
			allocation[allocation_top].reg_idx = i;
			allocation_top++;
			in_reg_queue(i);
			flag_reg[i] = 1;
			fprintf(out, "; ****\t");
			describe_quad_arg_to_file(arg);
			fprintf(out, "now in\t%s\n", reg_name[i]);
			return i;
		}
	// get one from reg_queue
	idx = out_reg_queue();
	// write the value of reg_idx to all the memory locations
	cancel_allocation(idx);
	// use it
	allocation[allocation_top].arg = arg;
	allocation[allocation_top].reg_idx = idx;
	allocation_top++;
	fprintf(out, "; ****\t");
	describe_quad_arg_to_file(arg);
	fprintf(out, "now in\t%s\n", reg_name[idx]);
	return idx;
}

void gen_jcc_with_temp_reg_all(int flag) {
	struct asm_arg_st *arg1, *arg2, *arg3;
	arg1 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg1->name = (char *) malloc(sizeof(char) * 256);
	arg2 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg2->name = (char *) malloc(sizeof(char) * 256);
	arg3 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg3->name = (char *) malloc(sizeof(char) * 256);
	//////////////////////////////////////////////////////////////////////////
	arg_memory_str(quadruple[quad_idx].arg1, arg1);
	fprintf(out, "\tmov eax, %s\n", arg1->name);
	arg_memory_str(quadruple[quad_idx].arg2, arg2);
	if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE)
		fprintf(out, "\tcmp eax, %s\n", arg2->name);
	else {
		fprintf(out, "\tmov ecx, %s\n", arg2->name);
		fprintf(out, "\tcmp eax, ecx\n");
	}
	//////////////////////////////////////////////////////////////////////////
	arg_memory_str(quadruple[quad_idx+1].arg1, arg1);
	switch (flag) {
		case QUAD_NEQL:
			fprintf(out, "\tje %s\n", arg1->name);
			break;
		case QUAD_EQL:
			fprintf(out, "\tjne %s\n", arg1->name);
			break;
		case QUAD_LEQ:
			fprintf(out, "\tjg %s\n", arg1->name);
			break;
		case QUAD_LES:
			fprintf(out, "\tjge %s\n", arg1->name);
			break;
		case QUAD_GEQ:
			fprintf(out, "\tjl %s\n", arg1->name);
			break;
		case QUAD_GTR:
			fprintf(out, "\tjle %s\n", arg1->name);
			break;
	}
}

void gen_add_sub_imul(int op) {
	int reg_idx1, reg_idx2, i;
	char *op_str;
	struct asm_arg_st *arg1, *arg2;
	arg1 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg1->name = (char *) malloc(sizeof(char) * 256);
	arg2 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg2->name = (char *) malloc(sizeof(char) * 256);
	//////////////////////////////////////////////////////////////////////////
	// it will overwrite the first arg
	if ((reg_idx1 = allocated(quadruple[quad_idx].arg1)) != -1)
		no_cancel_writeback(reg_idx1);
	else {
		reg_idx1 = get_reg_idx_to_allocate(quadruple[quad_idx].arg1);
		arg_memory_str(quadruple[quad_idx].arg1, arg1);
		fprintf(out, "\tmov %s, %s\n", reg_name[reg_idx1], arg1->name);
	}
	//
	op_str = (char *) malloc(sizeof(char) * 256);
	switch (op) {
		case QUAD_ADD:
			strcpy(op_str, "add");
			break;
		case QUAD_SUB:
			strcpy(op_str, "sub");
			break;
		case QUAD_MULT:
			strcpy(op_str, "imul");
			break;
	}
	if ((reg_idx2 = allocated(quadruple[quad_idx].arg2)) != -1)
		fprintf(out, "\t%s %s, %s\n", op_str, reg_name[reg_idx1], reg_name[reg_idx2]);
	else {
		reg_idx2 = get_reg_idx_to_allocate(quadruple[quad_idx].arg2);
		arg_memory_str(quadruple[quad_idx].arg2, arg2);
		fprintf(out, "\t%s %s, %s\n", op_str, reg_name[reg_idx1], arg2->name);
	}
	// the former allocation no longer exists
	for (i = 0; i < allocation_top; i++)
		if (same_arg(allocation[i].arg, quadruple[quad_idx].arg1)) {
			allocation[i].arg.arg_code = 0;
			allocation[i].reg_idx = -1;
		}	
		// new allocation occurs
		allocation[allocation_top].arg = quadruple[quad_idx].result;
		allocation[allocation_top].reg_idx = reg_idx1;
		allocation_top++;
}

void gen_asm_with_temp_reg_all() {
	int i, caller_depth, callee_depth, count;
	int reg_idx1, reg_idx2, tmp;
	struct asm_arg_st *arg1, *arg2, *arg3;
	out = fopen(asm_path, "w");
	prog_head();
	// init temp_reg_allocation
	allocation_top = 0;
	head_idx = 0;
	tail_idx = 0;
	for (i = REG_EAX; i <= REG_EBP; i++)
		allocation[i].arg.arg_code = 0;
	for (quad_idx = 0; quad_idx < quadruple_top; quad_idx++) {
		// quadruple information
		fprintf(out, "; %s\t", map_quad_string[quadruple[quad_idx].op]);
		describe_quad_arg_to_file(quadruple[quad_idx].arg1);
		describe_quad_arg_to_file(quadruple[quad_idx].arg2);
		describe_quad_arg_to_file(quadruple[quad_idx].result);
		fprintf(out, "\n");
		// initialize x86 instruction arguments
		arg1 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg1->name = (char *) malloc(sizeof(char) * 256);
		arg2 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg2->name = (char *) malloc(sizeof(char) * 256);
		arg3 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg3->name = (char *) malloc(sizeof(char) * 256);
		// translate
		switch(quadruple[quad_idx].op) {
			case QUAD_ASSIGN:
				// assign a immediate ?
				if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE) {
					reg_idx1 = get_reg_idx_to_allocate(quadruple[quad_idx].arg1);
					fprintf(out, "\tmov %s, %d\n", reg_name[reg_idx1], quadruple[quad_idx].arg2.val.int_val);
					break;
				}
				// arg2 allocated ?
				if ((reg_idx2 = allocated(quadruple[quad_idx].arg2)) != -1)
					flag_reg[reg_idx2] = 0;
				else {
					reg_idx2 = get_reg_idx_to_allocate(quadruple[quad_idx].arg2);
					arg_memory_str(quadruple[quad_idx].arg2, arg2);
					fprintf(out, "\tmov %s, %s\n", reg_name[reg_idx2], arg2->name);
				}
				allocation[allocation_top].arg = quadruple[quad_idx].arg1;
				allocation[allocation_top].reg_idx = reg_idx2;
				allocation_top++;
				break;
			case QUAD_ADD:
			case QUAD_SUB:
			case QUAD_MULT:
				gen_add_sub_imul(quadruple[quad_idx].op);
				break;
			case QUAD_DIV:
				// for conservation, free all regs
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				// edx:eax
				fprintf(out, "\tmov edx, 0\n");
				flag_reg[3] = 1;
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tmov eax, %s\n", arg1->name);
				flag_reg[0] = 1;
				arg_memory_str(quadruple[quad_idx].arg2, arg2);
				// mov arg2 to register
				tmp = get_reg_idx();
				fprintf(out, "\tmov %s, %s\n", reg_name[tmp], arg2->name);
				fprintf(out, "\tidiv %s\n", reg_name[tmp]);
				// mov eax to memory
				arg_memory_str(quadruple[quad_idx].result, arg3);
				// result is placed in eax
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_CALL:
				// when call occurs, free all allocations
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				// push the address of return value
				arg_memory_str(quadruple[quad_idx].arg2, arg2);
				fprintf(out, "\tlea eax, %s\n", arg2->name);
				fprintf(out, "\tpush eax\n");
				// the depth is that of the <sub_program>
				// add one to the depth of the procedure symbol
				caller_depth = cur_procedure.arg1.symbol_item->depth + 1;
				callee_depth = quadruple[quad_idx].arg1.symbol_item->depth + 1;
				if (callee_depth == caller_depth + 1) {
					// display to 1 ~> i - 1
					for (i = caller_depth; i > 1; i--)
						fprintf(out, "\tpush dword ptr [ebp+%d]\n", 4 * i);
					// display to i
					fprintf(out, "\tpush ebp\n");
				} else if (callee_depth <= caller_depth) {
					// display to 1 ~> j - 1
					for (i = callee_depth; i > 1; i--)
						fprintf(out, "\tpush dword ptr [ebp+%d]\n", 4 * i);
				}
				fprintf(out, "\tcall %s\n", quadruple[quad_idx].arg1.symbol_item->name);
				break;
			case QUAD_PROCMARK:
				cur_procedure = quadruple[quad_idx];
				fprintf(out, "%s\tPROC\n", quadruple[quad_idx].arg1.symbol_item->name);
				fprintf(out, "; *** prologue ***\n");
				fprintf(out, "\tpush\tebp\n");
				fprintf(out, "\tmov ebp, esp\n");
				// make room for locals & temps
				count = quadruple[quad_idx].arg2.val.int_val + quadruple[quad_idx].result.val.int_val;
				fprintf(out, "\tsub esp, %d\n", count * 4);
				fprintf(out, "; temps allocated between ebp-%d and ebp-%d\n",
					(quadruple[quad_idx].arg2.val.int_val + 1) * 4, count * 4);
				fprintf(out, "; *** subroutine body ***\n");
				break;
			case QUAD_PROCEND:
				// when call occurs, free all allocations
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				fprintf(out, "; *** epilogue ***\n");
				fprintf(out, "\tmov esp, ebp\n");
				fprintf(out, "\tpop ebp\n");
				if (!strcmp(quadruple[quad_idx].arg1.symbol_item->name, "start")) {
					fprintf(out, "\tpush offset WriteLine\n\tcall crt_printf\n");
					fprintf(out, "\tcall ExitProcess\n");
				}
				else
					fprintf(out, "\tret\n");
				fprintf(out, "%s\tENDP\n", quadruple[quad_idx].arg1.symbol_item->name);
				break;
			case QUAD_PARAMVAL:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tpush %s\n", arg1->name);
				break;
			case QUAD_PARAMREF:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				fprintf(out, "\tpush eax\n");
				break;
			case QUAD_READ:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				fprintf(out, "\tpush eax\n");
				if (quadruple[quad_idx].arg1.symbol_item->type_code == TYPE_INTEGER)
					fprintf(out, "\tpush offset OneInt\n");
				else
					fprintf(out, "\tpush offset OneChar\n");
				fprintf(out, "\tcall crt_scanf\n");
				break;
			case QUAD_WRITE:
				// when call occurs, free all allocations
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				if (quadruple[quad_idx].arg1.arg_code == ARG_STRING) {
					arg_memory_str(quadruple[quad_idx].arg1, arg1);
					fprintf(out, "\tpush offset %s\n", arg1->name);
					fprintf(out, "\tpush offset String\n");
					fprintf(out, "\tcall crt_printf\n");
				} else if (quadruple[quad_idx].arg1.arg_code == ARG_SYMBOL) {
					if ((tmp = allocated(quadruple[quad_idx].arg1)) != -1)
						fprintf(out, "\tpush %s\n", reg_name[tmp]);
					else {
						arg_memory_str(quadruple[quad_idx].arg1, arg1);
						fprintf(out, "\tpush %s\n", arg1->name);
					}
					if (quadruple[quad_idx].arg1.symbol_item->type_code == TYPE_INTEGER)
						fprintf(out, "\tpush offset OneInt\n");
					else
						fprintf(out, "\tpush offset OneChar\n");
					fprintf(out, "\tcall crt_printf\n");
				} else {
					arg_memory_str(quadruple[quad_idx].arg1, arg1);
					fprintf(out, "\tpush %s\n", arg1->name);
					fprintf(out, "\tpush offset OneInt\n");
					fprintf(out, "\tcall crt_printf\n");
				}
				if (quadruple[quad_idx].arg2.arg_code) {
					if (quadruple[quad_idx].arg2.arg_code == ARG_STRING) {
						arg_memory_str(quadruple[quad_idx].arg2, arg2);
						fprintf(out, "\tpush offset %s\n", arg2->name);
						fprintf(out, "\tpush offset String\n");
						fprintf(out, "\tcall crt_printf\n");
					} else {
						if ((tmp = allocated(quadruple[quad_idx].arg2)) != -1)
							fprintf(out, "\tpush %s\n", reg_name[tmp]);
						else {
							arg_memory_str(quadruple[quad_idx].arg2, arg2);
							fprintf(out, "\tpush %s\n", arg2->name);
						}
						fprintf(out, "\tpush offset OneInt\n");
						fprintf(out, "\tcall crt_printf\n");
					}
				}
				break;
			case QUAD_GETARRAY:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE)
					fprintf(out, "\tsub eax, %d\n", quadruple[quad_idx].arg2.val.int_val * 4);
				else {
					arg_memory_str(quadruple[quad_idx].arg2, arg2);
					fprintf(out, "\timul ecx, %s, 4\n", arg2->name);
					fprintf(out, "\tsub eax, ecx\n");
				}
				fprintf(out, "\tmov eax, dword ptr [eax]\n");
				arg_memory_str(quadruple[quad_idx].result, arg3);
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_SETARRAY:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE)
					fprintf(out, "\tsub eax, %d\n", quadruple[quad_idx].arg2.val.int_val * 4);
				else {
					arg_memory_str(quadruple[quad_idx].arg2, arg2);
					fprintf(out, "\timul ecx, %s, 4\n", arg2->name);
					fprintf(out, "\tsub eax, ecx\n");
				}
				arg_memory_str(quadruple[quad_idx].result, arg3);
				if (quadruple[quad_idx].result.arg_code == ARG_IMMEDIATE)
					fprintf(out, "\tmov dword ptr [eax], %s\n", arg3->name);
				else {
					fprintf(out, "\tmov ebx, %s\n", arg3->name);
					fprintf(out, "\tmov dword ptr [eax], ebx\n");
				}
				break;
			case QUAD_INC:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tinc %s\n", arg1->name);
				break;
			case QUAD_DEC:
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tdec %s\n", arg1->name);
				break;
			case QUAD_LABEL:
				// free all regs
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "%s:\n", arg1->name);
				break;
			case QUAD_NEQL:
			case QUAD_EQL:
			case QUAD_LEQ:
			case QUAD_LES:
			case QUAD_GEQ:
			case QUAD_GTR:
				// free all registers, cmp will use two
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				gen_jcc_with_temp_reg_all(quadruple[quad_idx].op);
				break;
			case QUAD_JMPF:
				break;
			case QUAD_JMP:
				for (i = 0; i < allocation_top; i++)
					cancel_allocation(i);
				free_regs();
				allocation_top = 0;
				arg_memory_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tjmp %s\n", arg1->name);
				break;
		}
	}
	fprintf(out, "END\n");
}

int get_reg_idx() {
	int i;
	for (i = 0; i < 6; i++)
		if (!flag_reg[i]) {
			flag_reg[i] = 1;
			return i;
		}
		return -1;
}

void asm_arg_str(t_quad_arg arg, struct asm_arg_st *asm_arg) {
	char *tmp;
	char *s;
	int cur_depth, arg_depth, reg_idx;
	s = asm_arg->name;
	s[0] = '\0';
	switch (arg.arg_code) {
		case ARG_SYMBOL:
			// procedure
			if (arg.symbol_item->category_code == CATEGORY_PROCEDURE) {
				strcat(s, arg.symbol_item->name);
				break;
			}
			// function
			if (arg.symbol_item->category_code == CATEGORY_FUNCTION) {
				arg_depth = arg.symbol_item->depth;
				strcat(s, "dword ptr [ebp+");
				tmp = (char *) malloc(sizeof(char) * 256);
				// return address, display, return value address, params
				itoa((1 + arg_depth + 1) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				fprintf(out, "\tmov %s, %s\n", reg_name[reg_idx = get_reg_idx()], s);
				strcpy(s, "dword ptr [");
				strcat(s, reg_name[reg_idx]);
				strcat(s, "]");
				break;
			}
			// const
			if (arg.symbol_item->category_code == CATEGORY_CONST) {
				tmp = (char *) malloc(sizeof(char) * 256);
				itoa(arg.symbol_item->val.int_val, tmp, 10);
				strcat(s, tmp);
				break;
			}
			// parameter
			arg_depth = arg.symbol_item->depth;
			if (arg.symbol_item->category_code == CATEGORY_PARAMREF || arg.symbol_item->category_code == CATEGORY_PARAMVAL) {
				strcat(s, "dword ptr [ebp+");
				tmp = (char *) malloc(sizeof(char) * 256);
				// first parameter first
				// return address, display, return value address, params
				itoa(4 + (arg_depth - 1) * 4 + 4 + (cur_procedure.arg1.symbol_item->param_count - arg.symbol_item->param_idx + 1) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				if (arg.symbol_item->category_code == CATEGORY_PARAMREF) {					
					// allocate a free register to save address of the parameter
					reg_idx = get_reg_idx();
					fprintf(out, "\tmov %s, %s\n", reg_name[reg_idx], s);
					s[0] = '\0';
					strcat(s, "dword ptr [");
					strcat(s, reg_name[reg_idx]);
					strcat(s, "]");
					break;
				}
				break;
			}
			// local
			// the depth of cur_procedure is less than the depth of its content
			cur_depth = cur_procedure.arg1.symbol_item->depth + 1;
			if (arg_depth < cur_depth) {
				// abp to arg_depth layer
				reg_idx = get_reg_idx();
				fprintf(out, "\tmov %s, dword ptr [ebp+%d]\n", reg_name[reg_idx], (cur_depth - arg_depth + 1) * 4);
				strcat(s, "dword ptr [");
				strcat(s, reg_name[reg_idx]);
				strcat(s, "-");
				tmp = (char *) malloc(sizeof(char) * 256);
				itoa((arg.symbol_item->offset_byte) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				break;
			}
			strcat(s, "dword ptr [ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.symbol_item->offset_byte * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			break;
		case ARG_IMMEDIATE:
			itoa(arg.val.int_val, s, 10);
			break;
		case ARG_TEMP_IDX:
			strcat(s, "dword ptr [ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			// first temp is t0
			itoa((arg.val.int_val + 1 + cur_procedure.arg2.val.int_val) * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			break;
		case ARG_LABEL:
			strcat(s, "label");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.val.int_val, tmp, 10);
			strcat(s, tmp);
			break;
		case ARG_STRING:
			strcat(s, "string");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.val.int_val, tmp, 10);
			strcat(s, tmp);
			break;
	}
}

void gen_asm() {
	int i, caller_depth, callee_depth, reg_idx, count;
	struct asm_arg_st *arg1, *arg2, *arg3;
	out = fopen(asm_path, "w");
	prog_head();
	// init temp_reg_allocation
	allocation_top = 0;
	head_idx = 0;
	tail_idx = 0;
	for (i = REG_EAX; i <= REG_EBP; i++)
		allocation[i].arg.arg_code = 0;
	for (quad_idx = 0; quad_idx < quadruple_top; quad_idx++) {
		// quadruple information
		fprintf(out, "; %s\t", map_quad_string[quadruple[quad_idx].op]);
		describe_quad_arg_to_file(quadruple[quad_idx].arg1);
		describe_quad_arg_to_file(quadruple[quad_idx].arg2);
		fprintf(out, "\n");
		// initialize x86 instruction arguments
		arg1 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg1->name = (char *) malloc(sizeof(char) * 256);
		arg2 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg2->name = (char *) malloc(sizeof(char) * 256);
		arg3 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg3->name = (char *) malloc(sizeof(char) * 256);
		// translate
		switch(quadruple[quad_idx].op) {
			case QUAD_PROCMARK:
				cur_procedure = quadruple[quad_idx];
				fprintf(out, "%s\tPROC\n", quadruple[quad_idx].arg1.symbol_item->name);
				fprintf(out, "; *** prologue ***\n");
				fprintf(out, "\tpush\tebp\n");
				fprintf(out, "\tmov ebp, esp\n");
				// make room for locals & temps
				count = quadruple[quad_idx].arg2.val.int_val + quadruple[quad_idx].result.val.int_val;
				fprintf(out, "\tsub esp, %d\n", count * 4);
				fprintf(out, "; temps allocated between ebp-%d and ebp-%d\n",
					(quadruple[quad_idx].arg2.val.int_val + 1) * 4, count * 4);
				fprintf(out, "; *** subroutine body ***\n");
				break;
			case QUAD_PROCEND:
				fprintf(out, "; *** epilogue ***\n");
				fprintf(out, "\tmov esp, ebp\n");
				fprintf(out, "\tpop ebp\n");
				if (!strcmp(quadruple[quad_idx].arg1.symbol_item->name, "start")) {
					fprintf(out, "\tpush offset WriteLine\n\tcall crt_printf\n");
					fprintf(out, "\tcall ExitProcess\n");
				}
				else
					fprintf(out, "\tret\n");
				fprintf(out, "%s\tENDP\n", quadruple[quad_idx].arg1.symbol_item->name);
				break;
			case QUAD_PARAMVAL:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tpush %s\n", arg1->name);
				free_regs();
				break;
			case QUAD_PARAMREF:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				fprintf(out, "\tpush eax\n");
				free_regs();
				break;
			case QUAD_CALL:
				// push the address of return value
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				fprintf(out, "\tlea eax, %s\n", arg2->name);
				fprintf(out, "\tpush eax\n");
				// the depth is that of the <sub_program>
				// add one to the depth of the procedure symbol
				caller_depth = cur_procedure.arg1.symbol_item->depth + 1;
				callee_depth = quadruple[quad_idx].arg1.symbol_item->depth + 1;
				if (callee_depth == caller_depth + 1) {
					// display to 1 ~> i - 1
					for (i = caller_depth; i > 1; i--)
						fprintf(out, "\tpush dword ptr [ebp+%d]\n", 4 * i);
					// display to i
					fprintf(out, "\tpush ebp\n");
				} else if (callee_depth <= caller_depth) {
					// display to 1 ~> j - 1
					for (i = callee_depth; i > 1; i--)
						fprintf(out, "\tpush dword ptr [ebp+%d]\n", 4 * i);
				}
				fprintf(out, "\tcall %s\n", quadruple[quad_idx].arg1.symbol_item->name);
				free_regs();
				break;
			case QUAD_READ:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				fprintf(out, "\tpush eax\n");
				if (quadruple[quad_idx].arg1.symbol_item->type_code == TYPE_INTEGER)
					fprintf(out, "\tpush offset OneInt\n");
				else
					fprintf(out, "\tpush offset OneChar\n");
				fprintf(out, "\tcall crt_scanf\n");
				break;
			case QUAD_WRITE:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				if (quadruple[quad_idx].arg1.arg_code == ARG_STRING) {
					fprintf(out, "\tpush offset %s\n", arg1->name);
					fprintf(out, "\tpush offset String\n");
					fprintf(out, "\tcall crt_printf\n");
				} else if (quadruple[quad_idx].arg1.arg_code == ARG_SYMBOL) {
					fprintf(out, "\tpush %s\n", arg1->name);
					if (quadruple[quad_idx].arg1.symbol_item->type_code == TYPE_INTEGER)
						fprintf(out, "\tpush offset OneInt\n");
					else
						fprintf(out, "\tpush offset OneChar\n");
					fprintf(out, "\tcall crt_printf\n");
				} else {
					fprintf(out, "\tpush %s\n", arg1->name);
					fprintf(out, "\tpush offset OneInt\n");
					fprintf(out, "\tcall crt_printf\n");
				}
				if (quadruple[quad_idx].arg2.arg_code) {
					asm_arg_str(quadruple[quad_idx].arg2, arg2);
					if (quadruple[quad_idx].arg2.arg_code == ARG_STRING) {
						fprintf(out, "\tpush offset %s\n", arg2->name);
						fprintf(out, "\tpush offset String\n");
						fprintf(out, "\tcall crt_printf\n");
					} else {
						fprintf(out, "\tpush %s\n", arg2->name);
						fprintf(out, "\tpush offset OneInt\n");
						fprintf(out, "\tcall crt_printf\n");
					}
				}
				free_regs();
				break;
			case QUAD_GETARRAY:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE)
					fprintf(out, "\tsub eax, %d\n", quadruple[quad_idx].arg2.val.int_val * 4);
				else {
					asm_arg_str(quadruple[quad_idx].arg2, arg2);
					fprintf(out, "\timul ecx, %s, 4\n", arg2->name);
					fprintf(out, "\tsub eax, ecx\n");
				}
				fprintf(out, "\tmov eax, dword ptr [eax]\n");
				asm_arg_str(quadruple[quad_idx].result, arg3);
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_SETARRAY:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tlea eax, %s\n", arg1->name);
				if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE)
					fprintf(out, "\tsub eax, %d\n", quadruple[quad_idx].arg2.val.int_val * 4);
				else {
					asm_arg_str(quadruple[quad_idx].arg2, arg2);
					fprintf(out, "\timul ecx, %s, 4\n", arg2->name);
					fprintf(out, "\tsub eax, ecx\n");
				}
				asm_arg_str(quadruple[quad_idx].result, arg3);
				if (quadruple[quad_idx].result.arg_code == ARG_IMMEDIATE)
					fprintf(out, "\tmov dword ptr [eax], %s\n", arg3->name);
				else {
					fprintf(out, "\tmov ebx, %s\n", arg3->name);
					fprintf(out, "\tmov dword ptr [eax], ebx\n");
				}
				break;
			case QUAD_ASSIGN:
				if (quadruple[quad_idx].arg2.arg_code == ARG_IMMEDIATE) {
					asm_arg_str(quadruple[quad_idx].arg1, arg1);
					fprintf(out, "\tmov %s, %d\n", arg1->name, quadruple[quad_idx].arg2.val.int_val);
					break;
				}
				// mov arg2 to register
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				fprintf(out, "\tmov eax, %s\n", arg2->name);
				flag_reg[0] = 1;
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tmov %s, eax\n", arg1->name);
				free_regs();
				break;
			case QUAD_ADD:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tmov eax, %s\n", arg1->name);
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				fprintf(out, "\tadd eax, %s\n", arg2->name);
				asm_arg_str(quadruple[quad_idx].result, arg3);
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_SUB:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tmov eax, %s\n", arg1->name);
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				fprintf(out, "\tsub eax, %s\n", arg2->name);
				asm_arg_str(quadruple[quad_idx].result, arg3);
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_MULT:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tmov eax, %s\n", arg1->name);
				flag_reg[0] = 1;
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				fprintf(out, "\timul eax, %s\n", arg2->name);
				asm_arg_str(quadruple[quad_idx].result, arg3);
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_DIV:
				// edx:eax
				fprintf(out, "\tmov edx, 0\n");
				flag_reg[3] = 1;
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tmov eax, %s\n", arg1->name);
				flag_reg[0] = 1;
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				// mov arg2 to register
				reg_idx = get_reg_idx();
				fprintf(out, "\tmov %s, %s\n", reg_name[reg_idx], arg2->name);
				fprintf(out, "\tidiv %s\n", reg_name[reg_idx]);
				// mov eax to memory
				asm_arg_str(quadruple[quad_idx].result, arg3);
				// result is placed in eax
				fprintf(out, "\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_INC:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tinc %s\n", arg1->name);
				break;
			case QUAD_DEC:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tdec %s\n", arg1->name);
				break;
			case QUAD_LABEL:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "%s:\n", arg1->name);
				break;
			case QUAD_JMPF:
				break;
			case QUAD_NEQL:
			case QUAD_EQL:
			case QUAD_LEQ:
			case QUAD_LES:
			case QUAD_GEQ:
			case QUAD_GTR:
				gen_jcc(quadruple[quad_idx].op);
				free_regs();
				break;
			case QUAD_JMP:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				fprintf(out, "\tjmp %s\n", arg1->name);
				break;
		}
	}
	fprintf(out, "END\n");
}

void prog_head() {
	int i;
	fprintf(out, ".386\n");
	fprintf(out, ".model flat, stdcall\n\n");
	fprintf(out, "include \\masm32\\include\\kernel32.inc\n");
	fprintf(out, "include \\masm32\\include\\msvcrt.inc\n");
	fprintf(out, "includelib \\masm32\\lib\\msvcrt.lib\n");
	fprintf(out, "includelib \\masm32\\lib\\kernel32.lib\n\n");
	// .data
	fprintf(out, ".data\n");
	fprintf(out, "String db \"%%s\", 0\n");
	fprintf(out, "OneChar db \"%%c\", 0\n");
	fprintf(out, "OneInt db \"%%d\", 0\n");
	fprintf(out, "WriteLine db 0ah, 0\n");
	for (i = 0; i< string_count; i++)
		fprintf(out, "string%d db \"%s\", 0\n", i, string_values[i]);
	// .code
	fprintf(out, ".CODE\n");
}


void gen_jcc(int flag) {
	struct asm_arg_st *arg1, *arg2, *arg3;
	int arg_idx1, arg_idx2;
	arg1 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg1->name = (char *) malloc(sizeof(char) * 256);
	arg2 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg2->name = (char *) malloc(sizeof(char) * 256);
	arg3 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
	arg3->name = (char *) malloc(sizeof(char) * 256);
	asm_arg_str(quadruple[quad_idx].arg1, arg1);
	arg_idx1 = get_reg_idx();
	fprintf(out, "\tmov %s, %s\n", reg_name[arg_idx1], arg1->name);
	asm_arg_str(quadruple[quad_idx].arg2, arg2);
	arg_idx2 = get_reg_idx();
	fprintf(out, "\tmov %s, %s\n", reg_name[arg_idx2], arg2->name);
	fprintf(out, "\tcmp %s, %s\n", reg_name[arg_idx1], reg_name[arg_idx2]);
	asm_arg_str(quadruple[quad_idx+1].arg1, arg1);
	switch (flag) {
		case QUAD_NEQL:
			fprintf(out, "\tje %s\n", arg1->name);
			break;
		case QUAD_EQL:
			fprintf(out, "\tjne %s\n", arg1->name);
			break;
		case QUAD_LEQ:
			fprintf(out, "\tjg %s\n", arg1->name);
			break;
		case QUAD_LES:
			fprintf(out, "\tjge %s\n", arg1->name);
			break;
		case QUAD_GEQ:
			fprintf(out, "\tjl %s\n", arg1->name);
			break;
		case QUAD_GTR:
			fprintf(out, "\tjle %s\n", arg1->name);
			break;
	}
}

void describe_quad_arg_to_file(t_quad_arg arg) {
	switch (arg.arg_code) {
		case ARG_SYMBOL:
			fprintf(out, "%s\t", arg.symbol_item->name);
			break;
		case ARG_IMMEDIATE:
			fprintf(out, "%d\t", arg.val.int_val);
			break;
		case ARG_LABEL:
			fprintf(out, "%d\t", arg.val.int_val);
			break;
		case ARG_TEMP_IDX:
			fprintf(out, "t%d\t", arg.val.int_val);
			break;
		case ARG_STRING:
			fprintf(out, "%s\t", string_values[arg.val.int_val]);
			break;
		default:
			fprintf(out, "N/A\t");
	}
}
