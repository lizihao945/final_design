#ifndef QUADRUPLE_H
#define QUADRUPLE_H

#include "symbol_table.h"
#include <stdio.h>

#define QUAD_ADD 517
#define QUAD_SUB 518
#define QUAD_MULT 519
#define QUAD_DIV 520
#define QUAD_EQL 521
#define QUAD_NEQL 522
#define QUAD_GTR 523
#define QUAD_LES 524
#define QUAD_DEF 525
#define QUAD_PARAMVAL 526
#define QUAD_PARAMREF 527
#define QUAD_ARGU 528
#define QUAD_CALL 529
#define QUAD_RET 530
#define QUAD_ASSIGN 531
#define QUAD_GETARRAY 532
#define QUAD_SETARRAY 533
#define QUAD_LABEL 534
#define QUAD_JMP 535
#define QUAD_JMPF 536
#define QUAD_CTOI 537
#define QUAD_ITOC 538

struct quadruple_st {
	int op, arg1, arg2, result;
};
extern struct quadruple_st quadruple;
int quadruple_add(int arg1, int arg2);
int quadruple_mult(int arg1, int arg2);
#endif