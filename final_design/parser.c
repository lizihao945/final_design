#include"lexer.h"
#include"parser.h"
#include "debug_helper_function.h"

void parse_const_def() {
	int i = idx;
	if (token.sy != IDEN) {

	}
	get_token_with_history();
	if (token.sy != EQL) {

	}
	if (token.sy == PLUS || token.sy == MINU) {
		get_token_with_history();
		if (token.sy == INTCON) {
			get_token_with_history();
		} else {
			eval_error(UNACCEPTABLE, "+/- not followed by INTCON");
			return;
		}
	} else if (token.sy == INTCON) {
		get_token_with_history();
	} else if (token.sy == CHARCON) {
		get_token_with_history();
	} else {
		eval_error(UNACCEPTABLE, "invalid const");
		return;
	}
	describe_token_history(i - 1, idx);
	print_verbose("<const_def> parsed");
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
		eval_error(UNACCEPTABLE, "<str> not started with a string");
	}
	get_token_with_history();
	describe_token_history(idx - 1, idx);
	print_verbose("<str> parsed");
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
	print_verbose("<read> parsed");
}

void parse_optread() {
	if (token.sy == COMMA) {
		get_token_with_history();
		parse_id();
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
	if (token.sy == IFTK)
		parse_if_statement();
	else
		parse_var();
	print_verbose("<statement> parsed");
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
			eval_error(RPARENT_MISSED, "parentheses should appear in pairs to represent a factor");
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
	int i = idx;
	parse_id();
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
		eval_error(RPARENT_MISSED, "in a argument list");
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
		eval_error(UNACCEPTABLE, "<if_statement> not started with 'if'");
		return;
	}
	get_token_with_history();
	parse_cond();
	if (token.sy != THENTK) {
		eval_error(UNACCEPTABLE, "no 'then' found after 'if'");
	}
	get_token_with_history();
	parse_statement();
	get_token_with_history();
	parse_else(i);
}

void parse_while_statement() {
	int i = idx;
	if (token.sy != WHILETK) {
		eval_error(UNACCEPTABLE, "<while_statement> not started with 'while'");
	}
	get_token_with_history();
	parse_cond();
	if (token.sy != DOTK) {
		eval_error(UNACCEPTABLE, "missing 'do' in a <while_statement>");
	}
	get_token_with_history();
	parse_statement();
	describe_token_history(i, idx);
	print_verbose("<while_statement> parsed");
}

void parse_for_statement() {
	int i = idx;
	if (token.sy != FORTK) {
		eval_error(UNACCEPTABLE, "<for_statement> not started with 'for'");
	}
	get_token_with_history();
	parse_id();
	if (token.sy != ASSIGN) {
		eval_error(UNACCEPTABLE, "missing ':=' in a <for_statement>");
	}
	get_token_with_history();
	parse_expression();
	if (token.sy != DOWNTOTK && token.sy != TOTK) {
		eval_error(UNACCEPTABLE, "missing 'downto | to' in a <for_statement>");
	}
	get_token_with_history();
	parse_expression();
	if (token.sy != DOWNTOTK && token.sy != DOTK) {
		eval_error(UNACCEPTABLE, "missing 'do' in a <for_statement>");
	}
	get_token_with_history();
	parse_statement();
	describe_token_history(i, idx);
	print_verbose("<for_statement> parsed");
}

parse_compound_statement() {
	int i = idx;
	if (token.sy != BEGINTK) {
		eval_error(UNACCEPTABLE, "<compound_statement> not started with 'begin'");
	}
	get_token_with_history();
	parse_statement();
	parse_optcompound_statement();
	if (token.sy != ENDTK) {
		eval_error(UNACCEPTABLE, "missing 'end' in a <compound_statement>");
	}
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
	test_argument();
	return 0;
}
