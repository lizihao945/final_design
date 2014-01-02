#ifndef LIVE_H
#define LIVE_H

#include "dag.h"
#include "debug_helper_function.h"
#include <stdio.h>
#include <stdlib.h>

struct var_list_st {
	int count;
	struct quad_arg_st val[1024];
};

void live_var_analysis();

#endif