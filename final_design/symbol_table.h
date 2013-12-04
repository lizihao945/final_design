#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>

#define MAX_SYM_NUM 1024
#define MAX_TEMP_NUM 1024

#define CATEGORY_CONST 511
#define CATEGORY_ARGUMENT 512
#define CATEGORY_ARRAY 513
#define CATEGORY_VARIABLE 514
#define CATEGORY_PROCEDURE 515
#define CATEGORY_FUNCTION 516

#define TYPE_INTEGER 517
#define TYPE_CHAR 518

#define TYPE_VAR_PARAMETER 519
#define TYPE_NON_VAR_PARAMETER 520

struct symbol_item_st {
	char name[256];
	int category_code;
	int type_code; // [array_of_] integer | char
	int param_type_code; // var or not
	union {
		char str_val[256];
		int int_val;
	} val;
	int offset_byte;
	int size_byte;// count of bytes
	int depth_val; // start from 1 as the very out layer
	int decl_line_num; // line number of declaration
	struct linked_ints_st *refs_line_num_list; // line numbers of references
	struct array_symbol_st *array_extra;
	struct func_symbol_st *func_extra;
	struct proc_symbol_st *proc_extra;
};

struct array_symbol_st {
	int array_dimension;
	int array_upper_bound;
};

struct func_symbol_st {
	int param_num;
	int size_byte;
};

struct proc_symbol_st {
	int param_num;
	int size_byte;
};

// global variables
extern struct symbol_item_st symbol_table[];
extern int symbol_table_top;
extern int temp_table_top;
int lookup_id(char name[]);
int push_symbol(int category_code, int type_code, char name[], int val);
int push_temp();
void fill_up_item(int item_idx, int category_code, int type_code, int val);

#endif