#ifndef LEXER_FIRST_H_INCLUDED
#define LEXER_FIRST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BEGINSY     1
#define ENDSY       2
#define IFSY        3
#define THENSY      4
#define ELSESY      5

#define IDSY        20
#define INTSY       21
#define PLUSY       22
#define MINUSY      23
#define STARSY      24
#define DIVISY      25
#define LPARSY      26
#define RPARSY      27
#define COMMASY     28
#define SEMISY      29
#define COLONSY     30
#define ASSIGNSY    31
#define EQUSY       32
#define LBRASY      33
#define RBRASY      34

#define LOWER_LETTER    111
#define UPPER_LETTER    112
#define DIGIT           113


struct token_sy {
    int sy;
    union {
        char strVal[256];
        int intVal;
        double doubleVal;
    } val;
};

int get_token(FILE *fp, struct token_sy *token);
int analyse_and_print(FILE *in);
#endif // LEXER_FIRST_H_INCLUDED
