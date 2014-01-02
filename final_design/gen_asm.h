#ifndef GEN_ASM_H
#define GEN_ASM_H
#include "quadruple.h"
#include "lexer.h"
#include "string.h"
#include "env.h"

#define REG_EAX 0
#define REG_ECX 1
#define REG_EDX 2
#define REG_ESP 3
#define REG_EBP 4
#define REG_EBX 5
#define REG_ESI 6
#define REG_EDI 7

struct asm_arg_st {
	char *name;
};

void gen_asm();
void gen_asm_with_temp_reg_all();

#endif