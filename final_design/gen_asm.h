#ifndef GEN_ASM_H
#define GEN_ASM_H
#include "quadruple.h"
#include "lexer.h"
#include "string.h"
#include "env.h"

#define REG_EAX 0
#define REG_EBX 1
#define REG_ECX 2
#define REG_EDX 3
#define REG_ESI 4
#define REG_EDI 5
#define REG_ESP 6
#define REG_EBP 7

struct asm_arg_st {
	char *name;
};

extern char * reg_name[8];
void gen_asm_with_reg_all();
void gen_asm();

#endif