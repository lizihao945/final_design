#include "debug_helper_function.h"
struct token_st token_history[MAX_TOKEN_NUM];
int idx = 0; // index of the token
int verbose_off = 0; // turn off verbose printing
int describe_token_off = 0; // turn off tokens of a N-T printing
const char * map_quad_string[1024];
const char * map_type_string[4];

void test_program() {
	char tmp;
	FILE *inn = fopen("tests/test_program.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		get_token_with_history();
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		parse_program();
		printf("******************\n");
		print_quadruples();
		fclose(in);
		remove("test.txt");
	}
}

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
		parse_var_part(2);
		print_symbol_table();
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
		parse_var_def(2);
		print_symbol_table();
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
	init_fake_symbol_table();
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		get_token_with_history();
		parse_for_statement();
		print_quadruples();
		fclose(in);
		remove("test.txt");
	}
}

void test_while_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_while_statement.txt", "r");
	init_fake_symbol_table();
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp != -1)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		get_token_with_history();
		parse_while_statement();
		print_quadruples();
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
		label_top = 0;
		temp_table_top = 0;
		quadruple_top = 0;
		get_token_with_history();
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
	init_fake_symbol_table();
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
	t_quad_arg *r;
	FILE *inn = fopen("tests/test_cond.txt", "r");
	init_fake_symbol_table();
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
		r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
		parse_cond(r);
		print_quadruples();
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
	for (i = 0; i < quadruple_top; i++) {
		if (quadruple[i].op == QUAD_LABEL) {
			printf("LABEL: %d\n", quadruple[i].arg1.val.int_val);
			continue;
		}
		printf("\t");
		if (strlen(map_quad_string[quadruple[i].op]) <= 7)
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
	switch (arg.arg_code) {
		case ARG_SYMBOL_IDX:
			printf("%s\t", symbol_table[arg.val.idx].name);
			break;
		case ARG_IMMEDIATE:
			printf("%d\t", arg.val.int_val);
			break;
		case ARG_LABEL:
			printf("%d\t", arg.val.int_val);
			break;
		case ARG_TEMP_IDX:
			printf("t%d\t", arg.val.int_val);
			break;
		case ARG_STRING:
			printf("%s\t", arg.val.str_val);
			break;
		default:
			printf("N/A\t");
	}
}

void init_fake_symbol_table() {
	symbol_table[symbol_table_top].category_code = CATEGORY_VARIABLE;
	symbol_table[symbol_table_top].type_code = TYPE_INTEGER;
	strcpy(symbol_table[symbol_table_top].name, "a");
	symbol_table[symbol_table_top].val.int_val = 1;
	symbol_table_top++;
	symbol_table[symbol_table_top].category_code = CATEGORY_VARIABLE;
	symbol_table[symbol_table_top].type_code = TYPE_INTEGER;
	strcpy(symbol_table[symbol_table_top].name, "b");
	symbol_table[symbol_table_top].val.int_val = 1;
	symbol_table_top++;
	symbol_table[symbol_table_top].category_code = CATEGORY_ARRAY;
	symbol_table[symbol_table_top].type_code = TYPE_INTEGER;
	symbol_table[symbol_table_top].upper_bound = 10;
	strcpy(symbol_table[symbol_table_top].name, "c");
	symbol_table[symbol_table_top].val.int_val = 1;
	symbol_table_top++;
}
void init_map_quad_string() {
	map_quad_string[QUAD_PROCEND] = "PROCEND";
	map_quad_string[QUAD_PROCMARK] = "PROCMARK";
	map_quad_string[QUAD_LEQ] = "LEQ<=";
	map_quad_string[QUAD_GEQ] = "GEQ>=";
	map_quad_string[QUAD_ADD] = "ADD+";
	map_quad_string[QUAD_SUB] = "SUB-";
	map_quad_string[QUAD_MULT] = "MULT*";
	map_quad_string[QUAD_DIV] = "DIV/";
	map_quad_string[QUAD_EQL] = "EQL=";
	map_quad_string[QUAD_NEQL] = "NEQL<>";
	map_quad_string[QUAD_GTR] = "GTR>";
	map_quad_string[QUAD_LES] = "LES<";
	map_quad_string[QUAD_DEF] = "DEF";
	map_quad_string[QUAD_PARAMVAL] = "PARAMVAL";
	map_quad_string[QUAD_PARAMREF] = "PARAMREF";
	map_quad_string[QUAD_ARGU] = "ARGU";
	map_quad_string[QUAD_CALL] = "CALL";
	map_quad_string[QUAD_RET] = "RET";
	map_quad_string[QUAD_ASSIGN] = "ASSIGN";
	map_quad_string[QUAD_GETARRAY] ="GETARRAY=[]";
	map_quad_string[QUAD_SETARRAY] ="SETARRAY[]=";
	map_quad_string[QUAD_LABEL] = "LABEL";
	map_quad_string[QUAD_JMP] = "JMP";
	map_quad_string[QUAD_JMPF] = "JMPF";
	map_quad_string[QUAD_CTOI] = "CTOI";
	map_quad_string[QUAD_ITOC] = "ITOC";
	map_quad_string[QUAD_WRITE] = "WRITE";
	map_quad_string[QUAD_READ] = "READ";
}

void print_symbol_table() {
	int i;
	for (i = 0; i < symbol_table_top; i++) {
		printf("name:%s\tcategory:%d\ttype:%s\tdepth:%d\n", symbol_table[i].name, symbol_table[i].category_code, map_type_string[symbol_table[i].type_code - 517], symbol_table[i].depth);
	}
}

void init_map_type_string() {
	map_type_string[TYPE_INTEGER - 517] = "integer";
	map_type_string[TYPE_CHAR - 517] = "char";
	map_type_string[TYPE_VAR_PARAMETER - 517] = "var_parameter";
	map_type_string[TYPE_NON_VAR_PARAMETER - 517] = "non_var_parameter";
}
