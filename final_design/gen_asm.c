#include "gen_asm.h"
int count;
void asm_arg_str(t_quad_arg arg, struct asm_arg_st *asm_arg) {
	char *tmp;
	char *s;
	s = asm_arg->name;
	s[0] = '\0';
	switch (arg.arg_code) {
		case ARG_SYMBOL:
			if (arg.symbol_item->category_code == CATEGORY_PROCEDURE) {
				strcat(s, arg.symbol_item->name);
				break;
			}
			strcat(s, "[ebp-");
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
			strcat(s, "[ebp-");
			tmp = (char *) malloc(sizeof(char) * 256);
			itoa((arg.symbol_item->offset_byte + count) * 4, tmp, 10);
			strcat(s, tmp);
			strcat(s, "]");
			asm_arg->arg_code = ASM_ARG_LOCAL;
			break;
	}
}

void gen_asm() {
	int i;
	struct asm_arg_st *arg1, *arg2, *arg3;
	printf(".386\n");
	printf(".model flat, stdcall\n\n");
	printf("include \\masm32\\include\\kernel32.inc\n");
	printf("include \\masm32\\include\\msvcrt.inc\n");
	printf("includelib \\masm32\\lib\\msvcrt.lib\n");
	printf("includelib \\masm32\\lib\\kernel32.lib\n\n");
	// .data
	printf(".data\n");
	printf("OneInt db \"%%d\", 0ah, 0\n");
	for (i = 0; i < quadruple_top; i++) {
		printf("; %s\t", map_quad_string[quadruple[i].op]);
		describe_quad_arg(quadruple[i].arg1);
		describe_quad_arg(quadruple[i].arg2);
		printf("\n");
		arg1 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg1->name = (char *) malloc(sizeof(char) * 256);
		arg2 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg2->name = (char *) malloc(sizeof(char) * 256);
		arg3 = (struct asm_arg_st *) malloc(sizeof(struct asm_arg_st));
		arg3->name = (char *) malloc(sizeof(char) * 256);
		asm_arg_str(quadruple[i].arg1, arg1);
		asm_arg_str(quadruple[i].arg2, arg2);
		asm_arg_str(quadruple[i].result, arg3);
		switch(quadruple[i].op) {
			case QUAD_PROCMARK:
				// code starts
				if (!strcmp(quadruple[i].arg1.val.str_val, "start")) {
					printf(".CODE\n");
					printf("PUBLIC start\n");
				}
				printf("%s\tPROC\n", quadruple[i].arg1.val.str_val);
				printf("; *** standard subroutine prologue ***\n");
				printf("\tpush\tebp\n");
				printf("\tmov ebp, esp\n");
				// make room for local variables
				count = quadruple[i].arg2.val.int_val;
				printf("\tsub esp, %d\n", count * 4);
				printf("\tpush\tedi\n");
				printf("\tpush\tesi\n");
				printf("; *** subroutine body ***\n");
				break;
			case QUAD_PROCEND:
				printf("; *** standard subroutine epilogue ***\n");
				printf("\tpop esi\n");
				printf("\tpop edi\n");
				printf("\tmov esp, ebp\n");
				if (!strcmp(quadruple[i].arg1.val.str_val, "start"))
					printf("\tcall ExitProcess\n");
				printf("%s\tENDP\n", quadruple[i].arg1.val.str_val);
				break;
			case QUAD_CALL:
				if (1)//cur_depth == 1)
					printf("\tcall %s\n", arg1->name);
				else {
					printf("\tmov eax, ebp\n");
					printf("\tcall %s\n", arg1->name);
				}
				break;
			case QUAD_WRITE:
				if (quadruple[i].arg2.arg_code)
					printf("\tpush %s\n", arg2->name);
				if (quadruple[i].arg1.arg_code)
					printf("\tpush %s\n", arg1->name);
				printf("\tpush offset OneInt\n");
				printf("\tcall crt_printf\n");
				break;
			case QUAD_ASSIGN:
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
				printf("\tmov eax, %s\n", arg1->name);
				printf("\tmov ebx, %s\n", arg2->name);
				printf("\tmul\tebx\n");
				printf("\tmov %s, eax\n", arg3->name);
				break;
			case QUAD_DIV:
				printf("\tmov eax, %s\n", arg1->name);
				printf("\tmov ebx, %s\n", arg2->name);
				printf("\tdiv\tebx\n");
				printf("\tmov %s, eax\n", arg3->name);
				break;
		}
	}

	printf("END\n");
}
