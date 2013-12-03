#include "symbol_table.h"
struct symbol_item symbol_table[MAX_SYM_NUM];
int symbol_table_top = 0;

int lookup_id(char name[256]) {
	int i = symbol_table_top - 1;
	while (i--)
		if (!strcmp(symbol_table[symbol_table_top].name, name))
			return i;
	return -1; // if not found
}

int push_item(int category_code, int type_code, char name[256], int val) {
	symbol_table[symbol_table_top].category_code = category_code;
	symbol_table[symbol_table_top].type_code = type_code;
	strcpy(symbol_table[symbol_table_top].name, name);
	symbol_table[symbol_table_top].val.int_val = val;
	return symbol_table_top++;
}

void fill_up_item() {

}
