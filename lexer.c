/*
	Zihao Li
	1106111
	Nov. 1st, 2013
*/
#include "lexer.h"

const char *map_sy_string[120];


int get_char_type(char ch) {
    if (ch >= 'a' && ch <= 'z') return LOWER_LETTER;
    if (ch >= 'A' && ch <= 'Z') return UPPER_LETTER;
    if (ch >= '0' && ch <= '9') return DIGIT;
    return ch;
}

int is_blank(char ch) {
    return (ch == ' ' || ch == '\n' || ch == '\t');
}

int is_letter_or_digit(char ch) {
    int charType = get_char_type(ch);
    return (charType == LOWER_LETTER || charType == UPPER_LETTER || charType == DIGIT);
}

int is_upper_letter(char ch) {
    return ('A' <= ch && ch <= 'Z') ? 1 : 0;
}

int get_token(FILE *fp, struct token_sy *token) {
    int i, ch;
    ch = getc(fp);
    while (is_blank(ch))
        ch = getc(fp);
    switch (get_char_type(ch)) {
    case LOWER_LETTER:
        token->sy = IDSY;
        token->val.strVal[0] = ch;
        for (i = 1; is_letter_or_digit(ch = fgetc(fp)); i++)
            token->val.strVal[i] = ch;
        ungetc(ch, fp);
        token->val.strVal[i] = '\0';
        return IDSY;
    case UPPER_LETTER:
        token->sy = IDSY;
        token->val.strVal[0] = ch;
        for (i = 1; is_upper_letter(ch = fgetc(fp)); i++)
            token->val.strVal[i] = ch;
        ungetc(ch, fp);
        token->val.strVal[i] = '\0';
        if (!strcmp(token->val.strVal, "IF")) {
            token->sy = IFSY;
            return IFSY;
        } else if (!strcmp(token->val.strVal, "THEN")) {
            token->sy = THENSY;
            return THENSY;
        } else if (!strcmp(token->val.strVal, "ELSE")) {
            token->sy = ELSESY;
            return ELSESY;
        }
        return IDSY;
    case DIGIT:
        token->sy = INTSY;
        char tmp[32];
        tmp[0] = ch;
        for (i = 1; get_char_type(ch = fgetc(fp)) == DIGIT; i++)
            tmp[i] = ch;
        tmp[i] = '\0';
        token->val.intVal = atoi(tmp);
        return INTSY;

    case '+':
        token->sy = PLUSY;
        strcpy(token->val.strVal, "+");
        return PLUSY;
    case '-':
        token->sy = MINUSY;
        strcpy(token->val.strVal, "-");
        return MINUSY;
    case '*':
        token->sy = STARSY;
        strcpy(token->val.strVal, "*");
        return STARSY;
    case '/':
        token->sy = DIVISY;
        strcpy(token->val.strVal, "/");
        return DIVISY;
    case '(':
        token->sy = LPARSY;
        strcpy(token->val.strVal, "(");
        return LPARSY;
    case ')':
        token->sy = RPARSY;
        strcpy(token->val.strVal, ")");
        return RPARSY;
    case ',':
        token->sy = COMMASY;
        strcpy(token->val.strVal, ",");
        return COMMASY;
    case ';':
        token->sy = SEMISY;
        strcpy( token->val.strVal, ";");
        return SEMISY;
    case ':':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = ASSIGNSY;
            strcpy(token->val.strVal, ":=");
            return ASSIGNSY;
        }
        ungetc(ch, fp);
        token->sy = COLONSY;
        strcpy(token->val.strVal, ":");
        return COLONSY;
    case '=':
        token->sy = EQUSY;
        strcpy(token->val.strVal, "=");
        return EQUSY;
    case '[':
        token->sy = LBRASY;
        strcpy(token->val.strVal, "[");
        return LBRASY;
    case ']':
        token->sy = RBRASY;
        strcpy(token->val.strVal, "]");
        return RBRASY;
    default:
        return -1;
    }
}

int analyse_and_print(FILE *in) {
    struct token_sy tmp;

    map_sy_string[1] = "BEGINSY";
    map_sy_string[2] = "ENDSY";
    map_sy_string[3] = "IFSY";
    map_sy_string[4] = "THENSY";
    map_sy_string[5] = "ELSESY";
    map_sy_string[20] = "IDSY";
    map_sy_string[21] = "INTSY";
    map_sy_string[22] = "PLUSY";
    map_sy_string[23] = "MINUSY";
    map_sy_string[24] = "STARSY";
    map_sy_string[25] = "DIVISY";
    map_sy_string[26] = "LPARSY";
    map_sy_string[27] = "RPARSY";
    map_sy_string[28] = "COMMASY";
    map_sy_string[29] = "SEMISY";
    map_sy_string[30] = "COLONSY";
    map_sy_string[31] = "ASSIGNSY";
    map_sy_string[32] = "EQUSY";
    map_sy_string[33] = "LBRASY";
    map_sy_string[34] = "RBRASY";

    map_sy_string[111] = "LOWER_LETTER";
    map_sy_string[112] = "UPPER_LETTER";
    map_sy_string[113] = "DIGIT";

    while (get_token(in, &tmp) != -1) {
        printf("%s", map_sy_string[tmp.sy]);
        if (tmp.sy == INTSY) printf("\t\t%d\n", tmp.val.intVal);
        else printf("\t\t%s\n", tmp.val.strVal);
    }
    return 0;
}
