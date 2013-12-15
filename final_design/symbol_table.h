#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <string.h>
#include <stdlib.h>
#include "errors.h"
#include "env.h"

#define CATEGORY_PARAMREF 509
#define CATEGORY_PARAMVAL 510
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
	char name[MAX_NAME];
	int category_code;
	int type_code; // [array_of_] integer | char
	int param_type_code; // var or not
	union {
		char str_val[MAX_NAME];
		int int_val;
	} val;
	int offset_byte;
	int size_byte;// count of bytes
	int depth; // start from 1 as the very out layer
	int decl_line_num; // line number of declaration
	int upper_bound; // in case it's array
	int param_num; // parameters are the first several symbols in the layer
};

// global variables
extern struct symbol_item_st symbol_table[];
extern int symbol_table_top;
extern int temp_table_top;
extern int sub_table_idx[];

// interfaces
int lookup_id(char name[]);
int push_symbol(int category_code, int type_code, char name[], int depth);
int push_temp();

#endif