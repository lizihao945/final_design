#include "debug_helper_function.h"
struct token_st token_history[MAX_TOKEN_NUM];
int idx = 0;
int verbose_off = 0;
int describe_token_off = 0;
const char * map_quad_string[1024];

void test_procedure_part() {
	char tmp;
	FILE *inn = fopen("tests/test_procedure_part.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_procedure_part();
		fclose(in);
		remove("test.txt");
	}
}

void test_procedure_head() {
	char tmp;
	FILE *inn = fopen("tests/test_procedure_head.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_procedure_head();
		fclose(in);
		remove("test.txt");
	}
}

void test_function_part()  {
	char tmp;
	FILE *inn = fopen("tests/test_function_part.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_function_part();
		fclose(in);
		remove("test.txt");
	}
}

void test_function_head() {
	char tmp;
	FILE *inn = fopen("tests/test_function_head.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_function_head();
		fclose(in);
		remove("test.txt");
	}
}

void test_parameter_list() {
	char tmp;
	FILE *inn = fopen("tests/test_parameter_list.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_parameter_list();
		fclose(in);
		remove("test.txt");
	}
}

void test_parameter() {
	char tmp;
	FILE *inn = fopen("tests/test_parameter.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_parameter();
		fclose(in);
		remove("test.txt");
	}
}
//******
void test_const_part() {
	char tmp;
	FILE *inn = fopen("tests/test_const_part.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_const_part();
		fclose(in);
		remove("test.txt");
	}
}

void test_const_def() {
	char tmp;
	FILE *inn = fopen("tests/test_const_def.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_const_def();
		fclose(in);
		remove("test.txt");
	}
}

void test_var_part() {
	char tmp;
	FILE *inn = fopen("tests/test_var_part.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_var_part();
		fclose(in);
		remove("test.txt");
	}
}

void test_var_def() {
	char tmp;
	FILE *inn = fopen("tests/test_var_def.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_var_def();
		fclose(in);
		remove("test.txt");
	}
}

void test_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_statement();
		fclose(in);
		remove("test.txt");
	}
}

void test_assign_statement() {
	char tmp;
	t_quad_arg p;
	FILE *inn = fopen("tests/test_assign_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		p.arg_code = ARG_SYMBOL_IDX;
		p.val.idx = 1;
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		parse_assign_statement(p);
		print_quadruples();
		fclose(in);
		remove("test.txt");
	}
}

void test_compound_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_compound_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_compound_statement();
		fclose(in);
		remove("test.txt");
	}
}

void test_for_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_for_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_for_statement();
		fclose(in);
		remove("test.txt");
	}
}

void test_while_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_while_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_while_statement();
		fclose(in);
		remove("test.txt");
	}
}

void test_expression() {
	char tmp;
	FILE *inn = fopen("tests/test_expression.txt", "r");
	t_quad_arg result;
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		parse_expression(&result);
		print_quadruples();
		fclose(in);
		remove("test.txt");
	}
}

void test_argument() {
	char tmp;
	FILE *inn = fopen("tests/test_argument.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_argument();
		fclose(in);
		remove("test.txt");
	}
}

void test_if_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_if_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1 && tmp >31)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		parse_if_statement();
		print_quadruples();
		fclose(in);
		remove("test.txt");
	}
}

void test_cond() {
	char tmp;
	FILE *inn = fopen("tests/test_cond.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_cond();
		fclose(in);
		remove("test.txt");
	}
}

void print_tokens(FILE *in) {
	struct token_st tmp;
	int token_count = 0;
	init_map_sy_string();
	while (get_token(in, &tmp) != 0) {
		printf("%d ", ++token_count);
		printf("%s ", map_sy_string[tmp.sy]);
		if (tmp.sy == INTCON) printf("%d\n", tmp.val.int_val);
		else printf("%s\n", tmp.val.str_val);
	}
}

void get_token_with_history() {
	// get_token should only be called here!
	if (get_token(in, &token) == -1) {
		token.sy = 0;
		strcpy(token.val.str_val, "#");
		token_history[idx++] = token;
		return;
	}
	token_history[idx++] = token;
}

void describe_token(struct token_st token) {
    if (token.sy == INTCON) printf("%d\t", token.val.int_val);
    else printf("%s\t", token.val.str_val);
}

void describe_token_history(int st, int en) {
    int i;
	if (describe_token_off) return;
    //since idx is ++ed every time a token is read,
    //the real index of last token should always - 1
    //and because of the pre-getting of token,
    //the end index should always - 1
    for(i = st - 1; i <= en - 2; i++)
        describe_token(token_history[i]);
    printf("\n");
}

void print_error(const char x[]) {
    printf("Error\t%s!\n", x);
//    printf("Error\t%s!\t Next token:\t%s\t", x, map_sy_string[token.sy]);
//    if (token.sy == INTCON)
//        printf("%d\n", token.val.intVal);
//    else
//        printf("%s\n", token.val.strVal);
}

void print_verbose(const char x[]) {
	if (verbose_off) return;
    printf("Verbose\t%s!\n", x);
//    printf("Verbose\t%s.\t Next token:\t%s\t", x, map_sy_string[token.sy]);
//    if (token.sy == INTCON)
//        printf("%d\n", token.val.intVal);
//    else
//        printf("%s\n", token.val.strVal);
}

void print_quadruples() {
	int i;
	init_map_quad_string();
	for (i = 0; i < quadruple_top; i++) {
		if (strlen(map_quad_string[quadruple[i].op]) <= 8)
			printf("%s\t\t", map_quad_string[quadruple[i].op]);
		else
			printf("%s\t", map_quad_string[quadruple[i].op]);
		describe_quad_arg(quadruple[i].arg1);
		describe_quad_arg(quadruple[i].arg2);
		describe_quad_arg(quadruple[i].result);
		printf("\n");
	}
}

void describe_quad_arg(t_quad_arg arg) {
	if (arg.arg_code == ARG_SYMBOL_IDX)
		printf("%s\t", symbol_table[arg.val.idx].name);
	else if (arg.arg_code == ARG_IMMEDIATE)
		printf("%d\t", arg.val.int_val);
	else if (arg.arg_code == ARG_LABEL)
		printf("%d\t", arg.val.int_val);
	else if (arg.arg_code == ARG_TEMP_IDX)
		printf("t%d\t", arg.val.int_val);
	else
		printf("N/A\t");
}

void init_map_quad_string() {
	map_quad_string[515] = "LEQ<=";
	map_quad_string[516] = "GEQ>=";
	map_quad_string[517] = "ADD+";
	map_quad_string[518] = "SUB-";
	map_quad_string[519] = "MULT*";
	map_quad_string[520] = "DIV/";
	map_quad_string[521] = "EQL=";
	map_quad_string[522] = "NEQL<>";
	map_quad_string[523] = "GTR>";
	map_quad_string[524] = "LES<";
	map_quad_string[525] = "DEF";
	map_quad_string[526] = "PARAMVAL";
	map_quad_string[527] = "PARAMREF";
	map_quad_string[528] = "ARGU";
	map_quad_string[529] = "CALL";
	map_quad_string[530] = "RET";
	map_quad_string[531] = "ASSIGN";
	map_quad_string[532] ="GETARRAY=[]";
	map_quad_string[533] ="SETARRAY[]=";
	map_quad_string[534] = "LABEL";
	map_quad_string[535] = "JMP";
	map_quad_string[536] = "JMPF";
	map_quad_string[537] = "CTOI";
	map_quad_string[538] = "ITOC";
}
