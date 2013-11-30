#include "errors.h"

int eval_error(int error_type, const char *p) {
	switch (error_type) {
		case ERR_INVALID_CHAR: print_error(p); break;
		case ERR_INVALID_ID: print_error(p); break;
		case ERR_UNACC_CHAR: print_error(p); break;
		case ERR_QMARK_MISSED:
			print_error("QMARK appeared not in pairs or TOO MUCH characters in QMARK pair");
			break;
		case ERR_DQMARK_MISSED: print_error(p); break;

		case ERR_TYPE_MISSMATCH_ON_ASSIGN: print_error(p); break;
		case ERR_TYPE_NOT_DEFINED: print_error(p); break;
		case ERR_TYPE_WRONG_ON_RETURN: print_error(p); break;
		case ERR_ID_REDEFINED: print_error(p); break;
		case ERR_ID_NOT_DEFINED: print_error(p); break;
		case ERR_RPARENT_MISSED: print_error(p); break;
		case ERR_RBRACK_MISSED: print_error(p); break;
		case ERR_END_MISSED: print_error(p); break;
		case ERR_ARG_TYPE_MISSMATCH: print_error(p); break;
		case ERR_ARG_NUM_MISSMATCH: print_error(p); break;
		case ERR_DIVIDED_BY_ZERO: print_error(p); break;
		case ERR_ASSIGN_TO_CONST: print_error(p); break;
		case ERR_RETURN_VAL_NOT_SET: print_error(p); break;
		case ERR_UNACCEPTABLE:
			print_error(p);
			break;
	}
	return 0;
}