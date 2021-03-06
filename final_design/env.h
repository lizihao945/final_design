#ifndef ENV_H
#define ENV_H

#include <stdio.h>

#define MAX_NAME 256 // identifier name length
#define MAX_SYM_NUM 1024 // maximum symbols
#define MAX_TEMP_NUM 1024 // maximum temps
#define MAX_QUAD_NUM 1024 // maximum quads
#define MAX_ASM_NUM 1024 // maximum asm instructions
#define MAX_SUB_DEPTH 64
#define ASM_REG_STR_LEN 256 // the length of the string of register

extern FILE *in;
extern FILE *out;
extern const char * asm_path;
#endif
