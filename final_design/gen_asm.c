#include "gen_asm.h"
int quad_idx;
t_quadruple cur_procedure;

int flag_reg[8]; // reg used?
char * reg_name[8] = {"eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp"};
int get_reg_idx();

void free_regs();
void prog_head();

/**
 * the value placed in regs in this procedure should not be freed
 * until the address of arg is calculated
 */
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
			// const
			if (arg.symbol_item->category_code == CATEGORY_CONST) {
				tmp = (char *) malloc(sizeof(char) * 256);
				itoa(arg.symbol_item->val.int_val, tmp, 10);
				strcat(s, tmp);
				break;
			}
			// parameter
			arg_depth = arg.symbol_item->depth;
			if (arg.symbol_item->category_code == CATEGORY_PARAMREF) {
				strcat(s, "dword ptr [ebp+");
				tmp = (char *) malloc(sizeof(char) * 256);
				// param_idx start from 0
				// return value is ebp+4
				// display allocated: ebp + 4 ~> ebp + depth * 4
				itoa((arg.symbol_item->param_idx + 1) * 4 + 4 + arg_depth * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				// allocate a free register to save address of the parameter
				reg_idx = get_reg_idx();
				printf("\tmov %s, %s\n", reg_name[reg_idx], s);
				s[0] = '\0';
				strcat(s, "dword ptr [");
				strcat(s, reg_name[reg_idx]);
				strcat(s, "]");
				asm_arg->arg_code = ASM_ARG_LOCAL;
				break;
			}
			if (arg.symbol_item->category_code == CATEGORY_PARAMVAL) {
				strcat(s, "dword ptr [ebp+");
				tmp = (char *) malloc(sizeof(char) * 256);
				// param_idx start from 0
				// return value is ebp+4
				// display allocated: ebp + 4 ~> ebp + depth * 4
				itoa((arg.symbol_item->param_idx + 1) * 4 + 4 + arg_depth * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				asm_arg->arg_code = ASM_ARG_LOCAL;
				break;
			}
			// local
			// the depth of cur_procedure is less than the depth of its content
			cur_depth = cur_procedure.arg1.symbol_item->depth + 1;
			if (arg_depth < cur_depth) {
				// abp to arg_depth layer
				reg_idx = get_reg_idx();
				printf("\tmov %s, dword ptr [ebp+%d]\n", reg_name[reg_idx], (arg_depth + 1) * 4);
				strcat(s, "dword ptr [");
				strcat(s, reg_name[reg_idx]);
				strcat(s, "-");
				tmp = (char *) malloc(sizeof(char) * 256);
				itoa((arg.symbol_item->offset_byte) * 4, tmp, 10);
				strcat(s, tmp);
				strcat(s, "]");
				asm_arg->arg_code = ASM_ARG_LOCAL;
				break;
			}
			strcat(s, "dword ptr [ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa(arg.symbol_item->offset_byte * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			asm_arg->arg_code = ASM_ARG_LOCAL;
			break;
		case ARG_IMMEDIATE:
			itoa(arg.val.int_val, s, 10);
			asm_arg->arg_code = ASM_ARG_IMMEDIATE;
			break;
		case ARG_TEMP_IDX:
			strcat(s, "dword ptr [ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			// first temp is t0
			itoa((arg.val.int_val + 1 + cur_procedure.arg2.val.int_val) * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			asm_arg->arg_code = ASM_ARG_LOCAL;
			break;
		case ARG_LABEL:
			strcat(s, "lable");
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
	prog_head();
	for (quad_idx = 0; quad_idx < quadruple_top; quad_idx++) {
		// quadruple information
		printf("; %s\t", map_quad_string[quadruple[quad_idx].op]);
		describe_quad_arg(quadruple[quad_idx].arg1);
		describe_quad_arg(quadruple[quad_idx].arg2);
		printf("\n");
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
				printf("%s\tPROC\n", quadruple[quad_idx].arg1.symbol_item->name);
				printf("; *** prologue ***\n");
				printf("\tpush\tebp\n");
				printf("\tmov ebp, esp\n");
				// make room for locals & temps
				count = quadruple[quad_idx].arg2.val.int_val + quadruple[quad_idx].result.val.int_val;
				printf("\tsub esp, %d\n", count * 4);
				printf("; temps allocated between ebp-%d and ebp-%d\n",
					(quadruple[quad_idx].arg2.val.int_val + 1) * 4, count * 4);
				printf("; *** subroutine body ***\n");
				break;
			case QUAD_PROCEND:
				printf("; *** epilogue ***\n");
				printf("\tmov esp, ebp\n");
				printf("\tpop ebp\n");
				if (!strcmp(quadruple[quad_idx].arg1.symbol_item->name, "start"))
					printf("\tcall ExitProcess\n");
				else
					printf("\tret\n");
				printf("%s\tENDP\n", quadruple[quad_idx].arg1.symbol_item->name);
				break;
			case QUAD_PARAMVAL:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tpush %s\n", arg1->name);
				free_regs();
				break;
			case QUAD_PARAMREF:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tlea eax, %s\n", arg1->name);
				printf("\tpush eax\n");
				free_regs();
				break;
			case QUAD_CALL:
				// the depth is that of the <sub_program>
				// add one to the depth of the procedure symbol
				caller_depth = cur_procedure.arg1.symbol_item->depth + 1;
				callee_depth = quadruple[quad_idx].arg1.symbol_item->depth + 1;
				if (callee_depth == caller_depth + 1) {
					// display to 1 ~> i - 1
					for (i = caller_depth; i > 1; i--)
						printf("\tpush dword ptr [ebp+%d]\n", 4 * i);
					// display to i
					printf("\tpush ebp\n");
				} else if (callee_depth <= caller_depth) {
					// display to 1 ~> j - 1
					for (i = callee_depth; i > 1; i--)
						printf("\tpush dword ptr [ebp+%d]\n", 4 * i);
				}
				printf("\tcall %s\n", quadruple[quad_idx].arg1.symbol_item->name);
				free_regs();
				break;
			case QUAD_WRITE:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				if (quadruple[quad_idx].arg1.arg_code == ARG_SYMBOL) {
					printf("\tpush %s\n", arg1->name);
					printf("\tpush offset OneInt\n");
					printf("\tcall crt_printf\n");
				} else if (quadruple[quad_idx].arg1.arg_code == ARG_STRING) {
					printf("\tpush offset %s\n", arg1->name);
					printf("\tpush offset String\n");
					printf("\tcall crt_printf\n");
				}
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				if (quadruple[quad_idx].arg2.arg_code == ARG_SYMBOL) {
					printf("\tpush %s\n", arg2->name);
					printf("\tpush offset OneInt\n");
					printf("\tcall crt_printf\n");
				} else if (quadruple[quad_idx].arg2.arg_code == ARG_STRING) {
					printf("\tpush offset %s\n", arg2->name);
					printf("\tpush offset String\n");
					printf("\tcall crt_printf\n");
				}
				// new line after write();
				printf("\tpush offset Writeline\n\tcall crt_printf\n");
				free_regs();
				break;
			case QUAD_ASSIGN:
				// mov arg2 to register
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				printf("\tmov eax, %s\n", arg2->name);
				flag_reg[0] = 1;
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tmov %s, eax\n", arg1->name);
				free_regs();
				break;
			case QUAD_ADD:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tmov eax, %s\n", arg1->name);
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				printf("\tadd eax, %s\n", arg2->name);
				asm_arg_str(quadruple[quad_idx].result, arg3);
				printf("\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_SUB:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tmov eax, %s\n", arg1->name);
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				printf("\tsub eax, %s\n", arg2->name);
				asm_arg_str(quadruple[quad_idx].result, arg3);
				printf("\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_MULT:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tmov eax, %s\n", arg1->name);
				flag_reg[0] = 1;
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				printf("\timul eax, %s\n", arg2->name);
				asm_arg_str(quadruple[quad_idx].result, arg3);
				printf("\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_DIV:
				// edx:eax
				printf("\tmov edx, 0\n");
				flag_reg[3] = 1;
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tmov eax, %s\n", arg1->name);
				flag_reg[0] = 1;
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				// mov arg2 to register
				reg_idx = get_reg_idx();
				printf("\tmov %s, %s\n", reg_name[reg_idx], arg2->name);
				printf("\tidiv %s\n", reg_name[reg_idx]);
				// mov eax to memory
				asm_arg_str(quadruple[quad_idx].result, arg3);
				// result is placed in eax
				printf("\tmov %s, eax\n", arg3->name);
				free_regs();
				break;
			case QUAD_LABEL:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("%s:\n", arg1->name);
				break;
			case QUAD_EQL:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tmov eax, %s\n", arg1->name);
				asm_arg_str(quadruple[quad_idx].arg2, arg2);
				printf("\tmov ebx, %s\n", arg2->name);
				printf("\tcmp eax, ebx\n");
				free_regs();
				break;
			case QUAD_JMP:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tjmp %s\n", arg1->name);
				break;
			case QUAD_JMPF:
				asm_arg_str(quadruple[quad_idx].arg1, arg1);
				printf("\tjne %s\n", arg1->name);
				break;
		}
	}

	printf("END\n");
}

void prog_head() {
	int i;
	printf(".386\n");
	printf(".model flat, stdcall\n\n");
	printf("include \\masm32\\include\\kernel32.inc\n");
	printf("include \\masm32\\include\\msvcrt.inc\n");
	printf("includelib \\masm32\\lib\\msvcrt.lib\n");
	printf("includelib \\masm32\\lib\\kernel32.lib\n\n");
	// .data
	printf(".data\n");
	printf("String db \"%%s\", 0\n");
	printf("OneInt db \"%%d\", 0\n");
	printf("WriteLine db 0ah, 0\n");
	for (i = 0; i< string_count; i++)
		printf("string%d db \"%s\", 0\n", i, string_values[i]);
	// .code
	printf(".CODE\n");
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

void free_regs() {
	int i;
	for (i = 0; i < 6; i++)
		flag_reg[i] = 0;
}