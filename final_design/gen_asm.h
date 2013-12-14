#ifndef GEN_ASM_H
#define GEN_ASM_H
#include "quadruple.h"
#include "lexer.h"
#include "string.h"
#include "env.h"
#include "debug_helper_function.h"

#define ASM_ARG_IMMEDIATE 2013
#define ASM_ARG_LOCAL 2014
#define ASM_ARG_REGISTER 2015
#define ASM_ARG_GLOBAL 2016

struct asm_arg_st {
	int arg_code;
	char *name;
};

void gen_asm();

#endif