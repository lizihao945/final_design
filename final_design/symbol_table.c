#include "symbol_table.h"
struct symbol_item symbol_table[MAX_SYM_NUM];
int symbol_table_top = 0;

int lookup_id(const char *str) {
	return -1; // if error occurs
}

void push_item(int category_code, int type_code, const char name[32], int val) {
	symbol_table[symbol_table_top].category_code = category_code;
	symbol_table[symbol_table_top].type_code = type_code;
	strcpy(symbol_table[symbol_table_top].name, name);
	symbol_table[symbol_table_top].val.int_val = val;
	symbol_table_top++;
}

void fill_up_item() {

}
