#include "debug_helper_function.h"
struct token_sy token_history[120];
int idx = 0;

void test_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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
	FILE *inn = fopen("tests/test_assign_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_assign_statement();
		fclose(in);
		remove("test.txt");
	}
}

void test_compound_statement() {
	char tmp;
	FILE *inn = fopen("tests/test_compound_statement.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_expression();
		fclose(in);
		remove("test.txt");
	}
}

void test_argument() {
	char tmp;
	FILE *inn = fopen("tests/test_argument.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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
		while ((tmp = fgetc(inn)) != '}' && tmp >31)
			fprintf(in, "%c", tmp);
		if (tmp == EOF) break;
		if (tmp <= 31) continue;
		fseek(in, 0, SEEK_SET);
		idx = 0;
		printf("******************\n");
		get_token_with_history();
		parse_if_statement();
		fclose(in);
		remove("test.txt");
	}
}

void test_const_def() {
	char tmp;
	FILE *inn = fopen("tests/test_const.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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

void test_cond() {
	char tmp;
	FILE *inn = fopen("tests/test_cond.txt", "r");
	while (!feof(inn)) {
		in = fopen("test.txt", "w+");
		while ((tmp = fgetc(inn)) != '}' && tmp > 31)
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
	struct token_sy tmp;
	int token_count = 0;
	init_map_sy_string();
	while (get_token(in, &tmp) != 0) {
		printf("%d ", ++token_count);
		printf("%s ", map_sy_string[tmp.sy]);
		if (tmp.sy == INTCON) printf("%d\n", tmp.val.intVal);
		else printf("%s\n", tmp.val.strVal);
	}
}

void get_token_with_history() {
	// get_token should only be called here!
	if (get_token(in, &token) ==- -1)
		return;
	token_history[idx++] = token;
}

void describe_token(struct token_sy token) {
    if (token.sy == INTCON) printf("%d\t", token.val.intVal);
    else printf("%s\t", token.val.strVal);
}

void print_action(char * const str, int * const vals) {
    printf("%s\t%d\t%d\t%d\n", str, vals[0], vals[1], vals[2]);
}

void describe_token_history(int st, int en) {
    int i;
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
    printf("Verbose\t%s!\n", x);
//    printf("Verbose\t%s.\t Next token:\t%s\t", x, map_sy_string[token.sy]);
//    if (token.sy == INTCON)
//        printf("%d\n", token.val.intVal);
//    else
//        printf("%s\n", token.val.strVal);
}
