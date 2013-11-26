#ifndef LEXER_FIRST_H_INCLUDED
#define LEXER_FIRST_H_INCLUDED
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"

#define IDEN        11
#define INTCON      21
#define CHARCON     41
#define STRCON      51
#define CONSTTK     61
#define INTTK       71
#define CHARTK      91
#define VARTK       101
#define ARRAYTK     111
#define OFTK        121
#define IFTK        32
#define THENTK      42
#define ELSETK      52
#define DOTK        62
#define WHILETK     72
#define FORTK       102
#define TOTK        112
#define DOWNTOTK    13
#define PROCETK     23
#define FUNCTK      33
#define READTK      43
#define WRITETK     53
#define BEGINTK     73
#define ENDTK       83
#define PLUS        103
#define MINU        113
#define MULT        14
#define DIV         24
#define LSS         34
#define LEQ         44
#define GRE         54
#define GEQ         64
#define EQL         74
#define NEQ         84
#define ASSIGN      94
#define SEMICN      104
#define COMMA       114
#define PERIOD      15
#define COLON       25
#define QMARK       35
#define DQMARK      45
#define LPARENT     55
#define RPARENT     65
#define LBRACK      75
#define RBRACK      85

#define LOWER_LETTER    111
#define UPPER_LETTER    112
#define DIGIT           113

extern FILE *in;
extern struct token_sy token;
extern const char *map_sy_string[120];

struct token_sy {
    int sy;
    union {
        char strVal[256];
        int intVal;
        double doubleVal;
    } val;
};

int get_token(FILE *fp, struct token_sy *token);
int analyse_and_print(FILE *in, FILE *out);
int init_map_sy_string();
#endif // LEXER_FIRST_H_INCLUDED
