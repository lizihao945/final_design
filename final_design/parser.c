#include"lexer.h"
#include"parser.h"
#include "debug_helper_function.h"

/**<常量>                   ::=    [+| -] <无符号整数>|<字符>
*/
void parse_const() {
	int i = idx;
	if (token.sy == PLUS || token.sy == MINU) {
		get_token_with_history();
		if (token.sy == INTCON) {
			get_token_with_history();
		} else {
			eval_error(UNACCEPTABLE);
			return;
		}
	} else if (token.sy == INTCON) {
		get_token_with_history();
	} else if (token.sy == CHARCON) {
		get_token_with_history();
	} else {
		eval_error(UNACCEPTABLE);
		return;
	}
	describe_token_history(i - 1, idx);
	print_verbose("a const parsed");
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
	print_verbose("a cond parsed");
}

void parse_str() {
	get_token_with_history();
	describe_token_history(idx - 1, idx);
	print_verbose("a string parsed");
}

void parse_id() {
	if (token.sy != IDEN)
		return;
	
	get_token_with_history();
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
	print_verbose("a write parsed");
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
	int i = idx;
	if (token.sy != READTK)
		return;
	get_token_with_history();
	if (token.sy != LPARENT)
		return;
	get_token_with_history();
	parse_id();
	parse_optread();
	if (token.sy != RPARENT)
		return;
	get_token_with_history();
	describe_token_history(i, idx);
	print_verbose("a read parsed");
}

void parse_optread() {
	if (token.sy == COMMA) {
		get_token_with_history();
		parse_id();
		parse_optread();
	}
}

void parse_if_statement() {
	if (token.sy != IFTK)
		return;
	get_token_with_history();
	parse_expression();
	if (token.sy != THENTK) {
		print_error("no THEN found after IF");
	}
	get_token_with_history();
	parse_statement();
	get_token_with_history();
	parse_else();
	print_verbose("a IF statement");
}

void parse_else() {
	if (token.sy != ELSETK)
		return;
	get_token_with_history();
	parse_statement();
	print_verbose("a IF statement with ELSE");
}

void parse_statement() {
	if (token.sy == IFTK)
		parse_if_statement();
	else
		parse_var();
	print_verbose("a statement");
}

void parse_term() {
	int i = idx;
	parse_factor();
	parse_terms();
	describe_token_history(i, idx);
	print_verbose("a term");
}

void parse_terms() {
	int i = idx;
	if (token.sy == MULT) {
		get_token_with_history();
		parse_factor();
		parse_terms();
		describe_token_history(i, idx);
		print_verbose("a terms(not empty)");
	} else {
		print_verbose("a terms(empty)");
	}
}

void parse_expression() {
	int i = idx;
	parse_term();
	parse_expressions();
	describe_token_history(i, idx);
	print_verbose("a expression");
}

void parse_expressions() {
	if (token.sy == PLUS || token.sy == MINU) {
		get_token_with_history();
		parse_term();
		parse_expressions();
	}
}

void parse_var() {
	int i = idx;
	parse_id();
	if (token.sy == LBRACK) {
		get_token_with_history();
		parse_expression();
		if (token.sy != RBRACK) {
			print_error("no RBARSY found after LBRASY");
		}
		get_token_with_history();
	} else {
		// nothing to do anymore because
		// a IDEN has been taken as a var()
	}
	describe_token_history(i, idx);
	print_verbose("a var");
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
			print_error("no RPARSY found after LPARSY");
		}
		get_token_with_history();
		break;
	default:
		break;
	}
	describe_token_history(i, idx);
	print_verbose("a factor");
}

int main() {
	//    char tmp[32], tmp2[32];
	//    printf("Input your source file name:\n");
	//    scanf("%s", tmp);
	init_map_sy_string();
	//print_tokens(in);
	test_cond();
	return 0;
}
