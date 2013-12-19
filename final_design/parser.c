#include "parser.h"

char last_proc_name[256];

int cur_depth;

void parse_program() {
	int *local_count;
	sub_table_idx[0] = -1;
	sub_table_idx[1] = 0;
	cur_depth = 1;
	push_symbol(CATEGORY_PROCEDURE, 0, "start", 0);
	strcpy(last_proc_name, "start");
	// no parameter for start
	local_count = (int *) malloc(sizeof(int));
	*local_count = 0;
	// no local or temp before start procedure
	parse_sub_program(0, local_count);
	if (token.sy != PERIOD)
		eval_error(ERR_UNACCEPTABLE, "missing '.' at the end of program");
	// get_token_with_history() is no longer needed
}

/**
 * symbol_idx shuold be the index of the proc symbol or -1 for "start"
 */
void parse_sub_program(int symbol_idx, int *local_count) {
	int mark_idx;
	t_quad_arg r;
	if (cur_depth > MAX_SUB_DEPTH) {
		eval_error(ERR_STACK_OVERFLOW, "");
		return;
	}
	if (token.sy == CONSTTK)
		parse_const_part();
	if (token.sy ==VARTK)
		parse_var_part(local_count);
	// a deeper layer start from the parameter_list
	if (token.sy == PROCETK) {
		parse_procedure_part(local_count);
		symbol_table_top = sub_table_idx[cur_depth--];
	}
	if (token.sy == FUNCTK) {
		parse_function_part(local_count);
		symbol_table_top = sub_table_idx[cur_depth--];
	}
	if (token.sy != BEGINTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'begin' in the program");
		return;
	}
	// generate procmark
	r.arg_code = ARG_SYMBOL;
	r.symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
	*(r.symbol_item) = symbol_table[symbol_idx];
	// procedure begins here, it should indicate 
	// the number of local variables
	mark_idx = quadruple_procmark(r, *local_count);
	// symbol table state in this procedure
	printf("symbol table state in: %s\n", r.symbol_item->name);
	print_symbol_table();
	printf("******************\n");
	temp_table_top = 0;
	// begin
	parse_compound_statement();
	// end
	quadruple[mark_idx].result.arg_code = ARG_IMMEDIATE;
	quadruple[mark_idx].result.val.int_val = temp_table_top;
	r.arg_code = ARG_SYMBOL;
	r.symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
	*(r.symbol_item) = symbol_table[symbol_idx];
	quadruple_procend(r);
	print_verbose("<sub_program> parsed");
}

void parse_id(char name[MAX_NAME]) {
	if (token.sy != IDEN)
		return;
	strcpy(name, token.val.str_val);
	get_token_with_history();
}

void parse_const_part() {
	int i = idx;
	if (token.sy != CONSTTK) {
		eval_error(ERR_UNACCEPTABLE, "<const_part> not started with 'const'");
	}
	get_token_with_history();
	parse_const_def();
	// (*local_count)++;
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_const_def();
		// (*local_count)++;
	}
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' int <const_part>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<const_part> parsed");
}

void parse_const_def() {
	char name[256];
	int const_type, flag = 0, val;
	int i = idx;
	parse_id(name);
	if (token.sy != EQL) {
		eval_error(ERR_UNACCEPTABLE, "missing '=' in <const_def>");
	}
	get_token_with_history();
	if (token.sy == PLUS || token.sy == MINU) {
		if (token.sy == MINU) flag = 1;
		get_token_with_history();
		if (token.sy == INTCON) {
			const_type = TYPE_INTEGER;
			val = flag ? -token.val.int_val : token.val.int_val;
			get_token_with_history();
		} else {
			eval_error(ERR_UNACCEPTABLE, "+/- not followed by INTCON");
			return;
		}
	} else if (token.sy == INTCON) {
		const_type = TYPE_INTEGER;
		val = token.val.int_val;
		get_token_with_history();
	} else if (token.sy == CHARCON) {
		const_type = TYPE_CHAR;
		val = token.val.int_val;
		get_token_with_history();
	} else {
		eval_error(ERR_UNACCEPTABLE, "invalid const value");
		return;
	}
	push_symbol(CATEGORY_CONST, const_type, name, 0);
	symbol_table[symbol_table_top - 1].val.int_val = val;
	describe_token_history(i, idx);
	print_verbose("<const_def> parsed");
}

void parse_var_part(int *local_count) {
	int i = idx;
	if (token.sy != VARTK) {
		eval_error(ERR_UNACCEPTABLE, "<var_part> not started with 'var'");
	}
	get_token_with_history();
	parse_var_def(local_count);
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <var_part>");
	} 
	get_token_with_history();
	while (token.sy == IDEN) {
		parse_var_def(local_count);
		if (token.sy != SEMICN) {
			eval_error(ERR_SEMICN_MISSED, "missing ';' in <var_part>");
		}
		get_token_with_history();
	}
	describe_token_history(i, idx);
	print_verbose("<var_part> parsed");
}

void parse_var_def(int *local_count) {
	struct linked_ints_st *stack, *top, *p;
	char name[256];
	int *category_type, *type_code, *upper_bound;
	int i = idx;
	parse_id(name);
	stack = (struct linked_ints_st *) malloc(sizeof(struct linked_ints_st));
	stack->val = push_symbol(0, 0, name, cur_depth);
	stack->next = NULL;
	top = stack;
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
		p = (struct linked_ints_st *) malloc(sizeof(struct linked_ints_st));
		p->val = push_symbol(0, 0, name, cur_depth);
		p->next = NULL;
		top->next = p;
		top = p;
	}
	if (token.sy != COLON) {
		eval_error(ERR_COLON_MISSED, "missing ':' in <var_def>");
	}
	get_token_with_history();
	category_type = (int *) malloc(sizeof(int *));
	type_code = (int *) malloc(sizeof(int *));
	upper_bound = (int *) malloc(sizeof(int *));
	parse_type(category_type, type_code, upper_bound);
	while (stack != NULL) {
		(*local_count)++;
		symbol_table[stack->val].offset_byte = *local_count;
		symbol_table[stack->val].category_code = *category_type;
		symbol_table[stack->val].type_code = *type_code;
		symbol_table[stack->val].upper_bound = *upper_bound;
		stack = stack->next;
		(*local_count) += *upper_bound;
	}
	describe_token_history(i, idx);
	print_verbose("<var_def> parsed");
}

void parse_type(int *category_type, int *type_code, int *upper_bound) {
	int i = idx;
	if (token.sy == ARRAYTK) {
		*category_type = CATEGORY_ARRAY;
		get_token_with_history();
		if (token.sy != LBRACK) {
			eval_error(ERR_LBRACK_MISSED, "missing '[' in <type>");
		}
		get_token_with_history();
		if (token.sy != INTCON) {
			eval_error(ERR_INVALID_ARRAY_IDX, "array index should be INTCON");
		}
		*upper_bound = token.val.int_val;
		get_token_with_history();
		if (token.sy != RBRACK) {
			eval_error(ERR_RBRACK_MISSED, "missing ']' in <type>");
		}
		get_token_with_history();
		if (token.sy != OFTK) {
			eval_error(ERR_UNACCEPTABLE, "missing 'of' in <type>");
		}
		get_token_with_history();
		parse_primitive_type(type_code);
	} else {
		parse_primitive_type(type_code);
		*category_type = CATEGORY_VARIABLE;
		*upper_bound = 0;
	}
	describe_token_history(i, idx);
	print_verbose("<type> parsed");
}

void parse_primitive_type(int *type_code) {
	if (token.sy != INTTK && token.sy != CHARTK){
		eval_error(ERR_UNACCEPTABLE, "<primitive_type> started with not a primitive type");
	}
	if (token.sy == INTTK) {
		*type_code = TYPE_INTEGER;
		get_token_with_history();
	} else {
		*type_code = TYPE_CHAR;
		get_token_with_history();
	}
}

void parse_procedure_part(int *local_count) {
	int i = idx;
	int *symbol_idx, *param_count;
	symbol_idx = (int *) malloc(sizeof(int));
	// the params are those of the sub_program
	param_count = (int *) malloc(sizeof(int));
	*param_count = 0;
	// make one place for the name of procedure
	sub_table_idx[++cur_depth] = symbol_table_top + 1;
	parse_procedure_head(symbol_idx, param_count);
	symbol_table[*symbol_idx].param_count = *param_count;
	// the procedure name counts
	(*local_count)++;
	// parameters are first locals
	parse_sub_program(*symbol_idx, param_count);
	*strrchr(last_proc_name, '_') = '\0';
	while (token.sy == SEMICN) {
		get_token_with_history();
		if (token.sy != PROCETK) {
			describe_token_history(i, idx);
			print_verbose("<procedure_part> parsed");
			return;
		}
		// the params are those of the sub_program
		param_count = (int *) malloc(sizeof(int));
		*param_count = 0;
		// make one place for the name of procedure
		sub_table_idx[++cur_depth] = symbol_table_top + 1;
		parse_procedure_head(symbol_idx, param_count);
		symbol_table[*symbol_idx].param_count = *param_count;
		// the procedure name counts
		(*local_count)++;
		parse_sub_program(*symbol_idx, param_count);
		*strrchr(last_proc_name, '_') = '\0';
	}
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <procedure_part>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<procedure_part> parsed");
}

void parse_procedure_head(int *symbol_idx, int *param_count) {
	char name[256], tmp[256];
	int i = idx;
	if (token.sy != PROCETK) {
		eval_error(ERR_UNACCEPTABLE, "<procedure_head> not started with 'procedure'");
	}
	get_token_with_history();
	parse_id(name);
	strcpy(tmp, last_proc_name);
	strcat(tmp, "_");
	strcat(tmp, name);
	strcpy(last_proc_name, tmp);
	*symbol_idx = push_symbol(CATEGORY_PROCEDURE, 0, tmp, cur_depth - 1);
	if (token.sy == LPARENT)
		parse_parameter_list(*symbol_idx, param_count);
	if (token.sy == COLON) {
		eval_error(ERR_UNACCEPTABLE, "procedure IDEN should not be specified with a type");
		return;
	}
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <procedure_head>");
		return;
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<procedure_head> parsed");
}

void parse_function_part(int *local_count) {
	int i = idx;
	int *symbol_idx, *param_count;
	symbol_idx = (int *) malloc(sizeof(int));
	// the params are those of the sub_program
	param_count = (int *) malloc(sizeof(int));
	*param_count = 0;
	// make one place for the name of function
	sub_table_idx[++cur_depth] = symbol_table_top + 1;
	parse_function_head(symbol_idx, param_count);
	symbol_table[*symbol_idx].param_count = *param_count;
	// the function name counts
	(*local_count)++;
	// parameters are first locals
	parse_sub_program(*symbol_idx, param_count);
	*strrchr(last_proc_name, '_') = '\0';
	while (token.sy == SEMICN) {
		get_token_with_history();
		if (token.sy != FUNCTK) {
			describe_token_history(i, idx);
			print_verbose("<function_part> parsed");
			return;
		}
		// the params are those of the sub_program
		param_count = (int *) malloc(sizeof(int));
		*param_count = 0;
		// make one place for the name of function
		sub_table_idx[++cur_depth] = symbol_table_top + 1;
		parse_function_head(symbol_idx, param_count);
		symbol_table[*symbol_idx].param_count = *param_count;
		// the function name counts
		(*local_count)++;
		parse_sub_program(*symbol_idx, param_count);
		*strrchr(last_proc_name, '_') = '\0';
	}
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <function_part>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<function_part> parsed");
}

void parse_function_head(int *symbol_idx, int *param_count) {
	char name[256], tmp[256];
	int i = idx;
	int *type_code;
	if (token.sy != FUNCTK) {
		eval_error(ERR_UNACCEPTABLE, "<function_head> not started with 'function'");
	}
	get_token_with_history();
	parse_id(name);
	strcpy(tmp, last_proc_name);
	strcat(tmp, "_");
	strcat(tmp, name);
	strcpy(last_proc_name, tmp);
	*symbol_idx = push_symbol(CATEGORY_FUNCTION, 0, tmp, cur_depth - 1);
	if (token.sy == LPARENT)
		parse_parameter_list(*symbol_idx, param_count);
	if (token.sy != COLON) {
		eval_error(ERR_UNACCEPTABLE, "missing ':' in <function_head>");
	}
	get_token_with_history();
	type_code = (int*) malloc(sizeof(int));
	parse_primitive_type(type_code);
	symbol_table[*symbol_idx].type_code = *type_code;
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <function_head>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<function_head> parsed");
}

void parse_parameter_list(int symbol_idx, int *param_count) {
	int i = idx;
	if (token.sy != LPARENT) {
		eval_error(ERR_UNACCEPTABLE, "<parameter_list> not started with '('");
	}
	get_token_with_history();
	if (token.sy == RPARENT) {
		eval_error(ERR_PARAMETER_MISSED, "use foo instead of foo()");
	}
	parse_parameter(symbol_idx, param_count);
	while (token.sy == SEMICN) {
		get_token_with_history();
		parse_parameter(symbol_idx, param_count);
	}
	if (token.sy != RPARENT) {
		eval_error(ERR_RPARENT_MISSED, "missing ')' in <parameter_list>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<parameter_list> parsed");
}

void parse_parameter(int symbol_idx, int *param_count) {
	char name[MAX_NAME];
	int *type_code;
	int i = idx;
	int flag = 0, st, tmp;
	if (token.sy == VARTK) {
		flag = 1;
		get_token_with_history();
	}
	// parameter list has many parts
	// start index of the symbol table of the procedure
	st = symbol_table_top;
	parse_id(name);
	if (flag)
		tmp = push_symbol(CATEGORY_PARAMREF, 0, name, cur_depth);
	else
		tmp = push_symbol(CATEGORY_PARAMVAL, 0, name, cur_depth);
	symbol_table[tmp].param_idx = *param_count;
	symbol_table[symbol_idx].param_symbol_idx[*param_count] = tmp;
	(*param_count)++;
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
		(*param_count)++;
		if (flag)
			tmp = push_symbol(CATEGORY_PARAMREF, 0, name, cur_depth);
		else
			tmp = push_symbol(CATEGORY_PARAMVAL, 0, name, cur_depth);
		symbol_table[tmp].param_idx = *param_count;
		symbol_table[symbol_idx].param_symbol_idx[*param_count] = tmp;
		(*param_count)++;
	}
	if (token.sy != COLON) {
		eval_error(ERR_COLON_MISSED, "missing ':' to indicate the type of IDEN");
		return;
	}
	get_token_with_history();
	type_code = (int*) malloc(sizeof(int));
	// fill back the type info
	parse_primitive_type(type_code);
	for (i = st; i < symbol_table_top; i++)
		symbol_table[i].type_code = *type_code;
	describe_token_history(i, idx);
	print_verbose("<parameter> parsed");
}

void parse_cond(t_quad_arg *r) {
	int i = idx;
	int tmp;
	t_quad_arg p, q;
	parse_expression(&p);
	tmp = token.sy;
	if (!(tmp & (LSS | LEQ | GRE | GEQ | EQL | NEQ)))
		eval_error(ERR_UNACCEPTABLE, "invalid operator");
	get_token_with_history();
	parse_expression(&q);
	switch (tmp) {
		case LSS:
			*r = quadruple_les(p, q);
			break;
		case LEQ:
			*r = quadruple_leq(p, q);
			break;
		case GRE:
			*r = quadruple_gtr(p, q);
			break;
		case GEQ:
			*r = quadruple_geq(p, q);
			break;
		case EQL:
			*r = quadruple_eql(p, q);
			break;
		case NEQ:
			*r = quadruple_neql(p, q);
			break;
		default:
			eval_error(ERR_UNACCEPTABLE, "invalid operator");
			break;
	}
	describe_token_history(i, idx);
	print_verbose("<cond> parsed");
}

void parse_str(t_quad_arg *r) {
	if (token.sy != STRCON) {
		eval_error(ERR_UNACCEPTABLE, "<str> not started with a string");
	}
	string_values[string_count] = (char *) malloc(sizeof(char) * 256);
	strcpy(string_values[string_count], token.val.str_val);
	r->arg_code = ARG_STRING;
	r->val.int_val = string_count;
	string_count++;
	get_token_with_history();
	describe_token_history(idx - 1, idx);
	print_verbose("<str> parsed");
}

void parse_write() {
	int i = idx;
	t_quad_arg *r1, *r2;
	if (token.sy != WRITETK)
		return;
	get_token_with_history();
	if (token.sy != LPARENT)
		return;
	get_token_with_history();
	r1 = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	r2 = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	r2->arg_code = 0;
	// firset arg
	if (token.sy == STRCON) {
		parse_str(r1);
		// second arg
		if (token.sy == COMMA) {
			get_token_with_history();
			parse_expression(r2);
			quadruple_write(*r1, *r2);
		} else {
			quadruple_write(*r1, *r2);
		}
	} else {
		r1 = (t_quad_arg *) malloc(sizeof(t_quad_arg));
		parse_expression(r1);
		quadruple_write(*r1, *r2);
	}
	// opt write over
	if (token.sy != RPARENT) {
		eval_error(ERR_UNACCEPTABLE, "')' missed in a <write> statement");
		return;
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<write> parsed");
}

void parse_read() {
	char name[256];
	int i = idx;
	t_quad_arg r1;
	if (token.sy != READTK)
		return;
	get_token_with_history();
	if (token.sy != LPARENT)
		return;
	get_token_with_history();
	parse_id(name);
	r1.arg_code = ARG_SYMBOL;
	r1.symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
	*(r1.symbol_item) = symbol_table[lookup_id(name)];
	quadruple_read(r1);
	parse_optread();
	if (token.sy != RPARENT)
		return;
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<read> parsed");
}

void parse_optread() {
	char name[256];
	t_quad_arg r1;
	if (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
		r1.arg_code = ARG_SYMBOL;
		r1.symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
		*(r1.symbol_item) = symbol_table[lookup_id(name)];
		quadruple_read(r1);
		parse_optread();
	}
}

void parse_assign_statement(t_quad_arg p) {
	int i = idx;
	t_quad_arg *q, *idx_val;
	if (token.sy == LBRACK) {
		get_token_with_history();
		idx_val = (t_quad_arg *) malloc(sizeof(t_quad_arg));
		// the index value of array
		parse_expression(idx_val);
		if (token.sy != RBRACK) {
			eval_error(ERR_RBRACK_MISSED, "square brackets should appear in pairs to indicate the index");
		}
		get_token_with_history();
		if (token.sy != ASSIGN) {
			eval_error(ERR_UNACCEPTABLE, "missing ':=' in a <assign_statement>");
		}
		get_token_with_history();
		q = (t_quad_arg *) malloc(sizeof(t_quad_arg));
		parse_expression(q);
		quadruple_setarray(p, *idx_val, *q);
		describe_token_history(i, idx);
		print_verbose("<assign_statement> parsed");
		return;
	}
	if (token.sy != ASSIGN) {
		eval_error(ERR_UNACCEPTABLE, "missing ':=' in a <assign_statement>");
	}
	get_token_with_history();
	q = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	q->arg_code = ARG_IMMEDIATE;
	parse_expression(q);
	quadruple_assign(p, *q);
	describe_token_history(i, idx);
	print_verbose("<assign_statement> parsed");
}

void parse_expression(t_quad_arg *r) {
	int i = idx;
	int flag = 0;
	t_quad_arg p, zero_arg;
	if (token.sy == PLUS || token.sy == MINU) {
		if (token.sy == MINU)
			flag = 1;
		get_token_with_history();
	}
	parse_term(&p);
	if (flag) {
		zero_arg.arg_code = ARG_IMMEDIATE;
		zero_arg.val.int_val = 0;
		parse_optexpression(quadruple_sub(zero_arg, p), r);
	} else
		parse_optexpression(p, r);
	describe_token_history(i, idx);
	print_verbose("<expression> parsed");
}

void parse_optexpression(t_quad_arg p, t_quad_arg *r) {
	int flag;
	t_quad_arg p1, q;
	if (token.sy == PLUS || token.sy == MINU) {
		flag = token.sy;
		get_token_with_history();
		parse_term(&q);
		if (flag == PLUS)
			p1 = quadruple_add(p, q);
		else
			p1 = quadruple_sub(p, q);
		parse_optexpression(p1, r);
	} else {
		*r = p;
	}
}

void parse_term(t_quad_arg *r) {
	int i = idx;
	t_quad_arg p;
	parse_factor(&p);
	parse_optterm(p, r);
	describe_token_history(i, idx);
	print_verbose("<term> parsed");
}

void parse_optterm(t_quad_arg p, t_quad_arg *r) {
	int flag;
	t_quad_arg p1, q;
	if (token.sy == MULT || token.sy == DIV) {
		flag = token.sy;
		get_token_with_history();
		parse_factor(&q);
		if (flag == MULT)
			p1 = quadruple_mult(p, q);
		else
			p1 = quadruple_div(p, q);
		parse_optterm(p1, r);
	} else {
		*r = p;
	}
}

void parse_factor(t_quad_arg *p)  {
	int i = idx;
	switch (token.sy) {
	case IDEN:
		parse_var(p);
		break;
	case INTCON:
		p->arg_code = ARG_IMMEDIATE;
		p->val.int_val = token.val.int_val;
		get_token_with_history();
		break;
	case LPARENT:
		get_token_with_history();
		parse_expression(p);
		if (token.sy != RPARENT) {
			eval_error(ERR_RPARENT_MISSED, "parentheses should appear in pairs to represent a factor");
		}
		get_token_with_history();
		break;
	default:
		break;
	}
	describe_token_history(i, idx);
	print_verbose("<factor> parsed");
}

void parse_var(t_quad_arg *p) {
	char name[MAX_NAME];
	int i = idx;
	int *ct, symbol_idx;
	t_quad_arg *q;
	q = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	parse_id(name);
	p->arg_code = ARG_SYMBOL;
	p->symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
	*(p->symbol_item) = symbol_table[lookup_id(name)];
	symbol_idx = lookup_id(name);
	if (token.sy == LBRACK) {
		get_token_with_history();
		parse_expression(q);
		*p = quadruple_getarray(*p, *q);
		if (token.sy != RBRACK) {
			print_error("no RBARSY found after LBRASY");
		}
		get_token_with_history();
	} else if (token.sy == LPARENT) {
		ct = (int *) malloc(sizeof(int));
		parse_argument(ct, symbol_idx);
		quadruple_call(*p, (*ct)+1);
	} else {
		// a IDEN has been taken as a var()
	}
	describe_token_history(i, idx);
	print_verbose("<var> parsed");
}

void parse_argument(int *ct, int symbol_idx) {
	int i = idx;
	t_quad_arg *r;
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	if (token.sy != LPARENT) {
		eval_error(ERR_UNACCEPTABLE, "<argument> started with not a '('");
	}
	get_token_with_history();
	// here is the first argument
	*ct = 0;
	parse_expression(r);
	if (symbol_table[symbol_table[symbol_idx].param_symbol_idx[*ct]].category_code == CATEGORY_PARAMVAL)
		quadruple_paramval(*r);
	else
		quadruple_paramref(*r);
	(*ct)++;
	parse_optargument(ct, symbol_idx);
	if (*ct == 0) {
		eval_error(ERR_UNACCEPTABLE, "no argument specified");
	}
	if (token.sy != RPARENT) {
		eval_error(ERR_RPARENT_MISSED, "in a argument list");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<argument> parsed");
}

void parse_optargument(int *ct, int symbol_idx) {
	t_quad_arg *r;
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	if (token.sy == COMMA) {
		(*ct)++;
		get_token_with_history();
		parse_expression(r);
		if (symbol_table[symbol_table[symbol_idx].param_symbol_idx[*ct]].category_code == CATEGORY_PARAMVAL)
			quadruple_paramval(*r);
		else
			quadruple_paramref(*r);
		parse_optargument(ct, symbol_idx);
	}
}

void parse_statement() {
	int i = idx;
	int *ct, symbol_idx;
	char name[MAX_NAME];
	t_quad_arg *p;
	switch (token.sy) {
		case IDEN:
			parse_id(name);
			symbol_idx = lookup_id(name);
			p = (t_quad_arg *) malloc(sizeof(t_quad_arg));
			p->arg_code = ARG_SYMBOL;
			p->symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
			*(p->symbol_item) = symbol_table[lookup_id(name)];
			if (token.sy == ASSIGN || token.sy == LBRACK) {
				if (p->symbol_item->category_code == CATEGORY_CONST ||
					p->symbol_item->category_code == CATEGORY_PROCEDURE)
					eval_error(ERR_UNACCEPTABLE, "left value can't be assigned");
				parse_assign_statement(*p);
			} else {
				if (token.sy == LPARENT) { // foo(1)
					ct = (int *) malloc(sizeof(int));
					parse_argument(ct, symbol_idx);
					quadruple_call(*p, (*ct)+1);
				} else { //foo
					if (p->symbol_item->param_count != 0) {
						eval_error(ERR_UNACCEPTABLE, "arguments not specified");
						return;
					}
					quadruple_call(*p, 0); // procedure call without return value
				}
			}
			break;
		case IFTK:
			parse_if_statement();
			break;
		case WHILETK:
			parse_while_statement();
			break;
		case BEGINTK:
			parse_compound_statement();
			break;
		case READTK:
			parse_read();
			break;
		case WRITETK:
			parse_write();
			break;
		case FORTK:
			parse_for_statement();
			break;
		default:
			// epsilon is allowed here
			break;
	}
	describe_token_history(i, idx);
	print_verbose("<statement> parsed");
}

void parse_if_statement() {
	int i = idx;
	int jmpf_idx, jmp_idx;
	t_quad_arg *r;
	if (token.sy != IFTK) {
		eval_error(ERR_UNACCEPTABLE, "<if_statement> not started with 'if'");
		return;
	}
	get_token_with_history();
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	parse_cond(r);
	jmpf_idx = quadruple_jmpf(*r);
	if (token.sy != THENTK) {
		eval_error(ERR_UNACCEPTABLE, "no 'then' found after 'if'");
	}
	get_token_with_history();
	parse_statement();
	if (token.sy != ELSETK) {
		quadruple[jmpf_idx].arg1.val.int_val = quadruple_label(); // label_a
		describe_token_history(i, idx);
		print_verbose("<if_statement> parsed without 'else'");
		return;
	}
	get_token_with_history();
	// return the index of QUAD_JMP
	jmp_idx = quadruple_jmp();
	quadruple[jmpf_idx].arg2 = *r;
	// set QUAD_JMPF result to label value(returned)
	// next quadruple should be labeled
	quadruple[jmpf_idx].arg1.val.int_val = quadruple_label();
	parse_statement();
	quadruple[jmp_idx].arg1.val.int_val = quadruple_label();
	describe_token_history(i, idx);
	print_verbose("<if_statement> parsed with 'else'");
}

void parse_while_statement() {
	int i = idx;
	int label_start, jmpf_idx;
	t_quad_arg *r;
	if (token.sy != WHILETK) {
		eval_error(ERR_UNACCEPTABLE, "<while_statement> not started with 'while'");
	}
	get_token_with_history();
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	// label_start
	label_start = quadruple_label();
	parse_cond(r);
	// if not satisfied, jump to the end
	jmpf_idx = quadruple_jmpf(*r);
	if (token.sy != DOTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'do' in a <while_statement>");
	}
	get_token_with_history();
	parse_statement();
	// loop
	quadruple[quadruple_jmp()].arg1.val.int_val = label_start;
	// label end
	quadruple[jmpf_idx].arg1.val.int_val = quadruple_label();
	describe_token_history(i, idx);
	print_verbose("<while_statement> parsed");
}

void parse_for_statement() {
	int i = idx;
	int tmp;
	int label_start, jmpf_idx;
	t_quad_arg p, *st, *ed, one_arg;
	if (token.sy != FORTK) {
		eval_error(ERR_UNACCEPTABLE, "<for_statement> not started with 'for'");
	}
	get_token_with_history();
	p.arg_code = ARG_SYMBOL;
	p.symbol_item = (struct symbol_item_st *) malloc(sizeof(struct symbol_item_st));
	*(p.symbol_item) = symbol_table[lookup_id(token.val.str_val)];
	get_token_with_history();
	if (token.sy != ASSIGN) {
		eval_error(ERR_UNACCEPTABLE, "missing ':=' in a <for_statement>");
	}
	get_token_with_history();
	// loop variable start value
	st = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	parse_expression(st);

	// assign the initial value to loop variable
	quadruple_assign(p, *st);

	// label_start
	label_start = quadruple_label();
	if (token.sy != DOWNTOTK && token.sy != TOTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'downto | to' in a <for_statement>");
	}
	tmp = token.sy;
	get_token_with_history();
	// loop variable start value
	ed = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	parse_expression(ed);

	// check loop variable before loop body
	one_arg.arg_code = ARG_IMMEDIATE;
	one_arg.val.int_val = 1;
	if (tmp == TOTK)
		jmpf_idx = quadruple_jmpf(quadruple_leq(p, *ed));
	else
		jmpf_idx = quadruple_jmpf(quadruple_geq(p, *ed));
	get_token_with_history();
	
	// loop body (<statement> and loop variable modification)
	parse_statement();
	if (tmp == TOTK)
		quadruple_inc(p);
	else
		quadruple_dec(p);
	quadruple[quadruple_jmp()].arg1.val.int_val = label_start;

	// label end
	quadruple[jmpf_idx].arg1.val.int_val = quadruple_label();
	describe_token_history(i, idx);
	print_verbose("<for_statement> parsed");
}

void parse_compound_statement() {
	int i = idx;
	if (token.sy != BEGINTK) {
		eval_error(ERR_UNACCEPTABLE, "<compound_statement> not started with 'begin'");
	}
	get_token_with_history();
	parse_statement();
	parse_optcompound_statement();
	if (token.sy != ENDTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'end' in a <compound_statement>, maybe you missed a ';'");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<compound_statement> parsed");
}

void parse_optcompound_statement() {
	if (token.sy == SEMICN) {
		get_token_with_history();
		if (token.sy == ENDTK)
			eval_error(ERR_UNACCEPTABLE, "the last <statement> before 'end' followed with ';'");
		parse_statement();
		parse_optcompound_statement();
	}
}

int main() {
	init_map_sy_string();
	init_map_type_string();
	init_map_quad_string();
	quadruple_top = 0;
	symbol_table_top = 0;
	do_compile_job();
	return 0;
}
