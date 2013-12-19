#include "symbol_table.h"
struct symbol_item_st symbol_table[MAX_SYM_NUM];
int symbol_table_top = 0;
int temp_table_top = 0;
int sub_table_idx[MAX_SUB_DEPTH];
char * string_values[MAX_SYM_NUM];
int string_count = 0;

int lookup_id(char name[MAX_NAME]) {
	char *tmp, *tmp1;
	int i = symbol_table_top;
	while (i-- >= 0) {
		tmp1 = strrchr(symbol_table[i].name, '_');
		if (!strcmp(symbol_table[i].name, name) || (tmp1 && !strcmp(tmp1 + 1, name)))
			return i;
	}
	tmp = (char *) malloc(sizeof(char) * 256);
	strcpy(tmp, "undefined variable ");
	strcat(tmp, name);
	eval_error(ERR_UNACCEPTABLE, tmp);
	return -1; // if not found
}

int push_symbol(int category_code, int type_code, char name[256], int depth) {
	symbol_table[symbol_table_top].category_code = category_code;
	symbol_table[symbol_table_top].type_code = type_code;
	strcpy(symbol_table[symbol_table_top].name, name);
	symbol_table[symbol_table_top].depth = depth;
	return symbol_table_top++;
}

int push_temp() {
	return temp_table_top++;
}
