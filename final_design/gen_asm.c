#include "gen_asm.h"
int count, quad_idx;
t_quadruple cur_procedure;

void prog_head();

void asm_arg_str(t_quad_arg arg, struct asm_arg_st *asm_arg, int pos) {
	char *tmp;
	char *s;
	int cur_depth, arg_depth;
	s = asm_arg->name;
	s[0] = '\0';
	switch (arg.arg_code) {
		case ARG_SYMBOL:
			// procedure
			if (arg.symbol_item->category_code == CATEGORY_PROCEDURE) {
				strcat(s, arg.symbol_item->name);
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
				printf("\tmov eax, %s\n", s);
				s[0] = '\0';
				strcpy(s, "dword ptr [eax]");
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
				printf("\tmov eax, dword ptr [ebp+%d]\n", (arg_depth + 1) * 4);
				strcat(s, "dword ptr [eax-");
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
			itoa((arg.val.int_val + count + 1) * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			asm_arg->arg_code = ASM_ARG_LOCAL;
			break;
	}
}

void gen_asm() {
	int i, caller_depth, callee_depth;
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
		asm_arg_str(quadruple[quad_idx].arg1, arg1, 1);
		asm_arg_str(quadruple[quad_idx].arg2, arg2, 2);
		asm_arg_str(quadruple[quad_idx].result, arg3, 3);
		// translate
		switch(quadruple[quad_idx].op) {
			case QUAD_PROCMARK:
				cur_procedure = quadruple[quad_idx];
				printf("%s\tPROC\n", quadruple[quad_idx].arg1.symbol_item->name);
				printf("; *** prologue ***\n");
				printf("\tpush\tebp\n");
				printf("\tmov ebp, esp\n");
				// make room for local variables
				count = quadruple[quad_idx].arg2.val.int_val;
				// count variables and prev ebp
				printf("\tsub esp, %d\n", count * 4);
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
				printf("\tpush %s\n", arg1->name);
				break;
			case QUAD_PARAMREF:
				printf("\tlea eax, %s\n", arg1->name);
				printf("\tpush eax\n");
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
				break;
			case QUAD_WRITE:
				if (quadruple[quad_idx].arg2.arg_code)
					printf("\tpush %s\n", arg2->name);
				if (quadruple[quad_idx].arg1.arg_code)
					printf("\tpush %s\n", arg1->name);
				printf("\tpush offset OneInt\n");
				printf("\tcall crt_printf\n");
				break;
			case QUAD_ASSIGN:
				// memory to memory
				if (arg1->arg_code == arg2->arg_code) {
					printf("\tmov eax, %s\n", arg2->name);
					printf("\tmov %s, eax\n", arg1->name);
				} else
					printf("\tmov %s, %s\n", arg1->name, arg2->name);
				break;
			case QUAD_ADD:
				printf("\tmov eax, %s\n", arg1->name);
				printf("\tadd eax, %s\n", arg2->name);
				printf("\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_SUB:
				printf("\tmov eax, %s\n", arg1->name);
				printf("\tsub eax, %s\n", arg2->name);
				printf("\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_MULT:
				printf("\timul eax, %s, %s\n", arg1->name, arg2->name);
				printf("\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_DIV:
				printf("\tmov eax, %s\n", arg1->name);
				printf("\tmov ebx, %s\n", arg2->name);
				printf("\tdiv\tebx\n");
				printf("\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_JMP:
				break;
			case QUAD_JMPF:
				break;
		}
	}

	printf("END\n");
}

void prog_head() {
	printf(".386\n");
	printf(".model flat, stdcall\n\n");
	printf("include \\masm32\\include\\kernel32.inc\n");
	printf("include \\masm32\\include\\msvcrt.inc\n");
	printf("includelib \\masm32\\lib\\msvcrt.lib\n");
	printf("includelib \\masm32\\lib\\kernel32.lib\n\n");
	// .data
	printf(".data\n");
	printf("OneInt db \"%%d\", 0ah, 0\n");
	// .code
	printf(".CODE\n");
}