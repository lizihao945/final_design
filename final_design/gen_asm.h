#ifndef GEN_ASM_H
#define GEN_ASM_H
#include "quadruple.h"
#include "lexer.h"
#include "string.h"
#include "env.h"

#define STR_LEN 256

#define ASM_ARG_IMMEDIATE 2013
#define ASM_ARG_ADDR 2014
#define ASM_ARG_REGISTER 2015
#define ASM_ARG_GLOBAL 2016

#define ASM_ADD 93
#define ASM_SUB 94
#define ASM_MOV 95
#define ASM_MUL 96
#define ASM_PUSH 97
#define ASM_POP 98

void gen_asm();

#endif