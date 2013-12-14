#include "gen_asm.h"
int flag = 0;

void asm_arg_str(t_quad_arg arg, char *s) {
	char *tmp;
	switch (arg.arg_code) {
		case ARG_SYMBOL_IDX:
			if (flag) {
				s[0] = '_';
				s[1] = '\0';
				strcat(s, symbol_table[arg.val.idx].name);
			} else {
				tmp = (char *) malloc(sizeof(char) * 256);
				s[0] = '_';
				s[1] = '\0';
				strcat(s, symbol_table[arg.val.idx].name);
				strcat(s, "$[ebp]");
			}				
			break;
		case ARG_IMMEDIATE:
			itoa(arg.val.int_val, s, 10);
			break;
		case ARG_TEMP_IDX:
			s[0] = '_';
			s[1] = 't';
			itoa(arg.val.idx, s + 2, 10);
			break;
	}
}

void gen_asm() {
	int i, j;
	char *s1, *s2, *s3;
	printf(".386\n");
	printf(".MODEL FLAT\n");
	for (i = 0; i < quadruple_top; i++) {
		s1 = (char *) malloc(sizeof(char) * 256);
		s2 = (char *) malloc(sizeof(char) * 256);
		s3 = (char *) malloc(sizeof(char) * 256);
		asm_arg_str(quadruple[i].arg1, s1);
		asm_arg_str(quadruple[i].arg2, s2);
		asm_arg_str(quadruple[i].result, s3);
		switch(quadruple[i].op) {
			case QUAD_PROCMARK:
				flag = 1;
				if (!strcmp(quadruple[i].arg1.val.str_val, "main")) {
					// global variables
					printf(".DATA\n");
					for (j = 0; j < symbol_table_top; j++)
						printf("\t_%s\tDD ?\n", symbol_table[j].name);
					for (j = 0; j < temp_table_top; j++)
						printf("\t_t%d\tDD ?\n", j);
					// code starts
					printf(".CODE\n");
				}
				printf("PUBLIC _%s\n", quadruple[i].arg1.val.str_val);
				printf("_%s\tPROC\n", quadruple[i].arg1.val.str_val);
				printf("; *** standard subroutine prologue ***\n");
				printf("\tpush\tebp\n");
				printf("\tmov ebp, esp\n");
				printf("\tsub esp, %d\n", quadruple[i].arg2.val.int_val * 4);
				printf("\tpush\tedi\n");
				printf("\tpush\tesi\n");
				printf("; *** subroutine body ***\n");
				break;
			case QUAD_PROCEND:
				printf("; *** standard subroutine epilogue ***\n");
				printf("\tpop esi\n");
				printf("\tpop edi\n");
				printf("\tmov esp, ebp\n");
				printf("_%s\tENDP\n", quadruple[i].arg1.val.str_val);
				break;
			case QUAD_ASSIGN:
				printf("\tmov %s, %s\n", s1, s2);
				break;
			case QUAD_ADD:
				printf("\tmov eax, %s\n", s1);
				printf("\tadd eax, %s\n", s2);
				printf("\tmov %s, eax\n", s3);
				break;
			case QUAD_SUB:
				printf("\tmov eax, %s\n", s1);
				printf("\tsub eax, %s\n", s2);
				printf("\tmov %s, eax\n", s3);
				break;
			case QUAD_MULT:
				printf("\tmov eax, %s\n", s1);
				printf("\tmov ebx, %s\n", s2);
				printf("\tmul\tebx\n");
				printf("\tmov %s, eax\n", s3);
				break;
			case QUAD_DIV:
				printf("\tmov eax, %s\n", s1);
				printf("\tmov ebx, %s\n", s2);
				printf("\tdiv\tebx\n");
				printf("\tmov %s, eax\n", s3);
				break;
		}
	}
	printf("END\n");
}
