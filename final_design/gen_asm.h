#ifndef GEN_ASM_H
#define GEN_ASM_H
#include "quadruple.h"
#include "lexer.h"
#include "string.h"
#include "env.h"
#include "debug_helper_function.h"

#define REG_EAX 0
#define REG_EBX 1
#define REG_ECX 2
#define REG_EDX 3
#define REG_ESI 4
#define REG_EDI 5
#define REG_ESP 6
#define REG_EBP 7

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