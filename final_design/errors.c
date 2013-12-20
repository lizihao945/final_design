#include "errors.h"

int eval_error(int error_type, const char *p) {
	print_error(p);
	exit(0);
}

void judge_type(t_quad_arg p, t_quad_arg q) {
	if (q.arg_code == ARG_IMMEDIATE) {
		if (p.symbol_item->type_code != TYPE_INTEGER)
			eval_error(ERR_UNACCEPTABLE, "type mismatch");
	} else if (q.arg_code == ARG_SYMBOL) {
		if (p.symbol_item->type_code == TYPE_CHAR && q.symbol_item->type_code == TYPE_INTEGER)
			eval_error(ERR_UNACCEPTABLE, "type mismatch");
	}
}
