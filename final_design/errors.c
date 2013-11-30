#include "errors.h"

int eval_error(int error_type, const char *p) {
	switch (error_type) {
		case INVALID_CHAR: print_error(p); break;
		case INVALID_ID: print_error(p); break;
		case UNACC_CHAR: print_error(p); break;
		case QMARK_MISSED:
			print_error("QMARK appeared not in pairs or TOO MUCH characters in QMARK pair");
			break;
		case DQMARK_MISSED: print_error(p); break;

		case TYPE_MISSMATCH_ON_ASSIGN: print_error(p); break;
		case TYPE_NOT_DEFINED: print_error(p); break;
		case TYPE_WRONG_ON_RETURN: print_error(p); break;
		case ID_REDEFINED: print_error(p); break;
		case ID_NOT_DEFINED: print_error(p); break;
		case RPARENT_MISSED: print_error(p); break;
		case RBRACK_MISSED: print_error(p); break;
		case END_MISSED: print_error(p); break;
		case ARG_TYPE_MISSMATCH: print_error(p); break;
		case ARG_NUM_MISSMATCH: print_error(p); break;
		case DIVIDED_BY_ZERO: print_error(p); break;
		case ASSIGN_TO_CONST: print_error(p); break;
		case RETURN_VAL_NOT_SET: print_error(p); break;
		case UNACCEPTABLE:
			print_error(p);
			break;
	}
	return 0;
}