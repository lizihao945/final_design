#include "parser.h"

void parse_program() {
	// sub_table_idx[0] is useless
	parse_sub_program(1);
	if (token.sy != PERIOD)
		eval_error(ERR_UNACCEPTABLE, "missing '.' at the end of program");
	// get_token_with_history() is no longer needed
}

void parse_sub_program(int depth) {
	sub_table_idx[depth] = symbol_table_top;
	if (depth > MAX_SUB_DEPTH) {
		eval_error(ERR_STACK_OVERFLOW, "");
		return;
	}
	if (token.sy == CONSTTK)
		parse_const_part();
	if (token.sy ==VARTK)
			parse_var_part(depth);
	if (token.sy == PROCETK)
			parse_procedure_part();
	if (token.sy == FUNCTK)
			parse_function_part();
	if (token.sy != BEGINTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'begin' in the program");
		return;
	}
	parse_compound_statement();
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
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_const_def();
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
	push_symbol(CATEGORY_CONST, const_type, name, val, 0);
	describe_token_history(i, idx);
	print_verbose("<const_def> parsed");
}

void parse_var_part(int depth) {
	int i = idx;
	if (token.sy != VARTK) {
		eval_error(ERR_UNACCEPTABLE, "<var_part> not started with 'var'");
	}
	get_token_with_history();
	parse_var_def(depth);
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <var_part>");
	} 
	get_token_with_history();
	while (token.sy == IDEN) {
		parse_var_def(depth);
		if (token.sy != SEMICN) {
			eval_error(ERR_SEMICN_MISSED, "missing ';' in <var_part>");
		}
		get_token_with_history();
	}
	describe_token_history(i, idx);
	print_verbose("<var_part> parsed");
}

void parse_var_def(int depth) {
	struct linked_ints_st *stack, *top, *p;
	char name[256];
	int *category_type, *type_code, *upper_bound;
	int i = idx;
	parse_id(name);
	stack = (struct linked_ints_st *) malloc(sizeof(struct linked_ints_st));
	stack->val = push_symbol(0, 0, name, 0, depth);
	stack->next = NULL;
	top = stack;
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
		p = (struct linked_ints_st *) malloc(sizeof(struct linked_ints_st));
		p->val = push_symbol(0, 0, name, 0, depth);
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
		fill_up_info(stack->val, *category_type, *type_code, *upper_bound);
		stack = stack->next;
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

void parse_procedure_part(int depth) {
	int i = idx;
	parse_procedure_head();
	parse_sub_program(depth + 1);
	while (token.sy == SEMICN) {
		get_token_with_history();
		parse_procedure_head();
		parse_sub_program(depth + 1);
	}
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <procedure_part>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<procedure_part> parsed");
}

void parse_procedure_head() {
	char name[256];
	int i = idx;
	if (token.sy != PROCETK) {
		eval_error(ERR_UNACCEPTABLE, "<procedure_head> not started with 'procedure'");
	}
	get_token_with_history();
	parse_id(name);
	if (token.sy == LPARENT)
		parse_parameter_list();
	if (token.sy == COLON) {
		eval_error(ERR_UNACCEPTABLE, "procedure IDEN should not be specified with a type");
		return;
	}
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <procedure_head>");
		return;
	}
	describe_token_history(i, idx);
	print_verbose("<procedure_head> parsed");
}

void parse_function_part(int depth) {
	int i = idx;
	parse_function_head();
	parse_sub_program(depth + 1);
	while (token.sy == SEMICN) {
		get_token_with_history();
		if (token.sy != FUNCTK) {
			describe_token_history(i, idx);
			print_verbose("<function_part> parsed");
			return;
		}
		parse_function_head();
		parse_sub_program(depth + 1);
	}
}

void parse_function_head() {
	char name[256];
	int i = idx;
	int *type_code;
	if (token.sy != FUNCTK) {
		eval_error(ERR_UNACCEPTABLE, "<function_head> not started with 'function'");
	}
	get_token_with_history();
	parse_id(name);
	if (token.sy == LPARENT)
		parse_parameter_list();
	if (token.sy != COLON) {
		eval_error(ERR_UNACCEPTABLE, "missing ':' in <function_head>");
	}
	get_token_with_history();
	type_code = (int*) malloc(sizeof(int));
	parse_primitive_type(type_code);
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <function_head>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<function_head> parsed");
}

void parse_parameter_list() {
	int i = idx;
	if (token.sy != LPARENT) {
		eval_error(ERR_UNACCEPTABLE, "<parameter_list> not started with '('");
	}
	get_token_with_history();
	if (token.sy == RPARENT) {
		eval_error(ERR_PARAMETER_MISSED, "");
	}
	parse_parameter();
	while (token.sy == SEMICN) {
		get_token_with_history();
		parse_parameter();
	}
	if (token.sy != RPARENT) {
		eval_error(ERR_RPARENT_MISSED, "missing ')' in <parameter_list>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<parameter_list> parsed");
}

void parse_parameter() {
	char name[256];
	int *type_code;
	int i = idx;
	if (token.sy == VARTK) { // reference passed in
		get_token_with_history();
	}
	parse_id(name);
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
	}
	if (token.sy != COLON) {
		eval_error(ERR_COLON_MISSED, "missing ':' to indicate the type of IDEN");
		return;
	}
	get_token_with_history();
	type_code = (int*) malloc(sizeof(int));
	parse_primitive_type(type_code);
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

void parse_str() {
	if (token.sy != STRCON) {
		eval_error(ERR_UNACCEPTABLE, "<str> not started with a string");
	}
	get_token_with_history();
	describe_token_history(idx - 1, idx);
	print_verbose("<str> parsed");
}

void parse_write() {
	int i = idx;
	if (token.sy != WRITETK)
		return;
	get_token_with_history();
	if (token.sy != LPARENT)
		return;
	get_token_with_history();
	parse_optwrite();
	if (token.sy != RPARENT)
		return;
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<write> parsed");
}

void parse_optwrite() {
	if (token.sy == STRCON) {
		parse_str();
		if (token.sy == COMMA) {
			get_token_with_history();
			parse_expression();
		} else {
		}
	} else
		parse_expression();
}

void parse_read() {
	char name[256];
	int i = idx;
	if (token.sy != READTK)
		return;
	get_token_with_history();
	if (token.sy != LPARENT)
		return;
	get_token_with_history();
	parse_id(name);
	parse_optread();
	if (token.sy != RPARENT)
		return;
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<read> parsed");
}

void parse_optread() {
	char name[256];
	if (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
		parse_optread();
	}
}

void parse_statement() {
	int i = idx;
	char name[MAX_NAME];
	t_quad_arg *p;
	switch (token.sy) {
		case IDEN:
			parse_id(name);
			p = (t_quad_arg *) malloc(sizeof(t_quad_arg));
			p->arg_code = ARG_SYMBOL_IDX;
			p->val.idx = lookup_id(name);
			if (token.sy == ASSIGN || token.sy == LBRACK) {
				parse_assign_statement(*p);
			} else {
				if (token.sy == LPARENT) { // foo(1)
					parse_argument();
				} else { //foo
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
	parse_optexpression(p, r);
	if (flag == 1) {
		zero_arg.arg_code = ARG_IMMEDIATE;
		zero_arg.val.int_val = 0;
		quadruple_sub(zero_arg, *r);
	}
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
	char name[256];
	int i = idx;
	t_quad_arg *q;
	q = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	parse_id(name);
	p->arg_code = ARG_SYMBOL_IDX;
	p->val.idx = lookup_id(name);
	if (token.sy == LBRACK) {
		get_token_with_history();
		parse_expression(q);
		*p = quadruple_getarray(*p, *q);
		if (token.sy != RBRACK) {
			print_error("no RBARSY found after LBRASY");
		}
		get_token_with_history();
	} else if (token.sy == LPARENT) {
		parse_argument();
	} else {
		// a IDEN has been taken as a var()
	}
	describe_token_history(i, idx);
	print_verbose("<var> parsed");
}

void parse_argument() {
	int i = idx;
	t_quad_arg *r;
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	if (token.sy != LPARENT) {

	}
	get_token_with_history();
	parse_expression(r);
	parse_optargument();
	if (token.sy != RPARENT) {
		eval_error(ERR_RPARENT_MISSED, "in a argument list");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<argument> parsed");
}

void parse_optargument() {
	t_quad_arg *r;
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	if (token.sy == COMMA) {
		get_token_with_history();
		parse_expression(r);
		parse_optargument();
	}
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
		quadruple[jmpf_idx].arg1.val.idx = quadruple_lable(); // label_a
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
	quadruple[jmpf_idx].arg1.val.idx = quadruple_lable();
	parse_statement();
	quadruple[jmp_idx].arg1.val.idx = quadruple_lable();
	describe_token_history(i, idx);
	print_verbose("<if_statement> parsed with 'else'");
}

void parse_while_statement() {
	int i = idx;
	t_quad_arg *r;
	if (token.sy != WHILETK) {
		eval_error(ERR_UNACCEPTABLE, "<while_statement> not started with 'while'");
	}
	get_token_with_history();
	r = (t_quad_arg *) malloc(sizeof(t_quad_arg));
	parse_cond(r);
	if (token.sy != DOTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'do' in a <while_statement>");
	}
	get_token_with_history();
	parse_statement();
	describe_token_history(i, idx);
	print_verbose("<while_statement> parsed");
}

void parse_for_statement() {
	int i = idx;
	int tmp;
	int label_start, label_end, jmpf_idx;
	t_quad_arg p, *st, *ed, one_arg;
	if (token.sy != FORTK) {
		eval_error(ERR_UNACCEPTABLE, "<for_statement> not started with 'for'");
	}
	get_token_with_history();
	p.arg_code = ARG_SYMBOL_IDX;
	p.val.idx = lookup_id(token.val.str_val);
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
	label_start = quadruple_lable();
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
	if (token.sy == DOTK)
		quadruple_add(p, one_arg);
	else
		quadruple_sub(p, one_arg);
	quadruple[quadruple_jmp()].arg1.val.int_val = label_start;

	// label end
	quadruple[jmpf_idx].arg1.val.int_val = quadruple_lable();
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
		eval_error(ERR_UNACCEPTABLE, "missing 'end' in a <compound_statement>");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<compound_statement> parsed");
}

void parse_optcompound_statement() {
	if (token.sy == SEMICN) {
		get_token_with_history();
		parse_statement();
		parse_optcompound_statement();
	}
}

int main() {
	//    char tmp[32], tmp2[32];
	//    printf("Input your source file name:\n");
	//    scanf("%s", tmp);
	init_map_sy_string();
	init_map_type_string();
	init_map_quad_string();
	//quadruple_top = 0;
	//symbol_table_top = 0;
	//print_tokens(in);
	
	verbose_off = 1;
	describe_token_off = 1;
	test_for_statement();
	//test_cond();
	//test_var_part();
	return 0;
}
