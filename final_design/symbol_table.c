#include "symbol_table.h"
struct symbol_item_st symbol_table[MAX_SYM_NUM];
int symbol_table_top = 0;
int temp_table_top = 0;
int sub_table_idx[MAX_SUB_DEPTH];

int lookup_id(char name[MAX_NAME]) {
	int i = symbol_table_top;
	while (i-- >= 0)
		if (!strcmp(symbol_table[i].name, name))
			return i;
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
