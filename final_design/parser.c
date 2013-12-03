#include "parser.h"

void parse_program() {
	parse_sub_program();
	if (token.sy != PERIOD)
		eval_error(ERR_UNACCEPTABLE, "missing '.' at the end of program");
	// get_token_with_history() is no longer needed
}

void parse_sub_program() {
	if (token.sy == CONSTTK)
		parse_const_part();
	if (token.sy ==VARTK)
			parse_var_part();
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

void parse_id(char str[256]) {
	if (token.sy != IDEN)
		return;
	strcpy(str, token.val.str_val);
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
	push_item(TYPE_CONST, const_type, name, val);
	describe_token_history(i, idx);
	print_verbose("<const_def> parsed");
}

void parse_var_part() {
	int i = idx;
	if (token.sy != VARTK) {
		eval_error(ERR_UNACCEPTABLE, "<var_part> not started with 'var'");
	}
	get_token_with_history();
	parse_var_def();
	if (token.sy != SEMICN) {
		eval_error(ERR_SEMICN_MISSED, "missing ';' in <var_part>");
	} 
	get_token_with_history();
	while (token.sy == IDEN) {
		parse_var_def();
		if (token.sy != SEMICN) {
			eval_error(ERR_SEMICN_MISSED, "missing ';' in <var_part>");
		}
		get_token_with_history();
	}
	describe_token_history(i, idx);
	print_verbose("<var_part> parsed");
}

void parse_var_def() {
	struct linked_ints *stack, *top, *p;
	char name[256];
	int *category_type, *type_code, *upper_bound;
	int i = idx;
	parse_id(name);
	stack = (struct linked_ints *) malloc(sizeof(struct linked_ints));
	stack->val = push_item(0, 0, name, 0);
	stack->next = NULL;
	top = stack;
	while (token.sy == COMMA) {
		get_token_with_history();
		parse_id(name);
		p = (struct linked_ints *) malloc(sizeof(struct linked_ints));
		p->val = push_item(0, 0, name, 0);
		p->next = NULL;
		top->next = p;
		top = p;
	}
	if (token.sy != COLON) {
		eval_error(ERR_COLON_MISSED, "missing ':' in <var_def>");
	}
	get_token_with_history();
	category_type = (int *) malloc(sizeof(int));
	type_code = (int *) malloc(sizeof(int));
	upper_bound = (int *) malloc(sizeof(int));
	parse_type(category_type, type_code, upper_bound);
	while (stack != NULL) {
		push_item(*category_type, *type_code, name, stack->val);
		stack = stack->next;
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<var_def> parsed");
}

void parse_type(int *category_type, int *type_code, int *upper_bound) {
	int i = idx;
	if (token.sy == ARRAYTK) {
		*category_type = TYPE_ARRAY;
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
	} else
		parse_primitive_type(type_code);
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

void parse_procedure_part() {
	int i = idx;
	parse_procedure_head();
	parse_sub_program();
	while (token.sy == SEMICN) {
		get_token_with_history();
		parse_procedure_head();
		parse_sub_program();
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

void parse_function_part() {
	int i = idx;
	parse_function_head();
	parse_sub_program();
	while (token.sy == SEMICN) {
		get_token_with_history();
		if (token.sy != FUNCTK) {
			describe_token_history(i, idx);
			print_verbose("<function_part> parsed");
			return;
		}
		parse_function_head();
		parse_sub_program();
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

void parse_cond() {
	int i = idx;
	parse_expression();
	switch (token.sy) {
		case LSS:
			get_token_with_history();
			break;
		case LEQ:
			get_token_with_history();
			break;
		case GRE:
			get_token_with_history();
			break;
		case GEQ:
			get_token_with_history();
			break;
		case EQL:
			get_token_with_history();
			break;
		case NEQ:
			get_token_with_history();
			break;
		default:
			break;
	}
	parse_expression();
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

/**
 * arg 'i' is for debug
 */
void parse_else(int i) {
	if (token.sy != ELSETK) {
		describe_token_history(i, idx);
		print_verbose("<if_statement> parsed without <else>");
		return;
	}
	get_token_with_history();
	parse_statement();
	describe_token_history(i, idx);
	print_verbose("<if_statement> parsed with <else>");
}

void parse_statement() {
	int i = idx;
	switch (token.sy) {
		case IDEN:
			get_token_with_history();
			if (token.sy == ASSIGN || token.sy == LBRACK) {
				parse_assign_statement();
			} else {
				if (token.sy == LPARENT) { // foo(1)
					parse_argument();
				} else { //foo
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

void parse_assign_statement() {
	int i = idx;
	if (token.sy == LBRACK) {
		get_token_with_history();
		parse_expression();
		if (token.sy != RBRACK) {
			eval_error(ERR_RBRACK_MISSED, "square brackets should appear in pairs to indicate the index");
		}
		get_token_with_history();
	}
	if (token.sy != ASSIGN) {
		eval_error(ERR_UNACCEPTABLE, "missing ':=' in a <assign_statement>");
	}
	get_token_with_history();
	parse_expression();
	describe_token_history(i, idx);
	print_verbose("<assign_statement> parsed");
}

void parse_expression() {
	int i = idx;
	if (token.sy == PLUS || token.sy == MINU) {
		get_token_with_history();
	}
	parse_term();
	parse_optexpression();
	describe_token_history(i, idx);
	print_verbose("<expression> parsed");
}

void parse_optexpression() {
	if (token.sy == PLUS || token.sy == MINU) {
		get_token_with_history();
		parse_term();
		parse_optexpression();
	}
}

void parse_term() {
	int i = idx;
	parse_factor();
	parse_optterm();
	describe_token_history(i, idx);
	print_verbose("<term> parsed");
}

void parse_optterm() {
	if (token.sy == MULT || token.sy == DIV) {
		get_token_with_history();
		parse_factor();
		parse_optterm();
	}
}

void parse_factor()  {
	int i = idx;
	switch (token.sy) {
	case IDEN:
		parse_var();
		break;
	case INTCON:
		get_token_with_history();
		break;
	case LPARENT:
		get_token_with_history();
		parse_expression();
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

void parse_var() {
	char name[256];
	int i = idx;
	parse_id(name);
	if (token.sy == LBRACK) {
		get_token_with_history();
		parse_expression();
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
	if (token.sy != LPARENT) {

	}
	get_token_with_history();
	parse_expression();
	parse_optargument();
	if (token.sy != RPARENT) {
		eval_error(ERR_RPARENT_MISSED, "in a argument list");
	}
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("<argument> parsed");
}

void parse_optargument() {
	if (token.sy == COMMA) {
		get_token_with_history();
		parse_expression();
		parse_optargument();
	}
}

void parse_if_statement() {
	int i = idx;
	if (token.sy != IFTK) {
		eval_error(ERR_UNACCEPTABLE, "<if_statement> not started with 'if'");
		return;
	}
	get_token_with_history();
	parse_cond();
	if (token.sy != THENTK) {
		eval_error(ERR_UNACCEPTABLE, "no 'then' found after 'if'");
	}
	get_token_with_history();
	parse_statement();
	get_token_with_history();
	parse_else(i);
}

void parse_while_statement() {
	int i = idx;
	if (token.sy != WHILETK) {
		eval_error(ERR_UNACCEPTABLE, "<while_statement> not started with 'while'");
	}
	get_token_with_history();
	parse_cond();
	if (token.sy != DOTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'do' in a <while_statement>");
	}
	get_token_with_history();
	parse_statement();
	describe_token_history(i, idx);
	print_verbose("<while_statement> parsed");
}

void parse_for_statement() {
	char name[256];
	int i = idx;
	if (token.sy != FORTK) {
		eval_error(ERR_UNACCEPTABLE, "<for_statement> not started with 'for'");
	}
	get_token_with_history();
	parse_id(name);
	if (token.sy != ASSIGN) {
		eval_error(ERR_UNACCEPTABLE, "missing ':=' in a <for_statement>");
	}
	get_token_with_history();
	parse_expression();
	if (token.sy != DOWNTOTK && token.sy != TOTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'downto | to' in a <for_statement>");
	}
	get_token_with_history();
	parse_expression();
	if (token.sy != DOWNTOTK && token.sy != DOTK) {
		eval_error(ERR_UNACCEPTABLE, "missing 'do' in a <for_statement>");
	}
	get_token_with_history();
	parse_statement();
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
	//print_tokens(in);
	test_const_def();

	return 0;
}
