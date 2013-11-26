#include "errors.h"

int eval_error(int error_type) {
	switch (error_type) {
		case QMARK_MISSED:
			print_error("QMARK appeared not in pairs or TOO MUCH characters in QMARK pair");
			break;
		case UNACCEPTABLE:
			print_error("not acceptable tokens in sequence");
			break;
	}
	return 0;
}