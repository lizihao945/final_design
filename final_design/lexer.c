/*
	Zihao Li
	1106111
	Nov. 1st, 2013
*/
#include "lexer.h"
FILE *in;
struct token_sy token;
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

int is_str_val(char ch) {
	return (ch == 32 || ch == 33 || (35 <= ch && ch <= 126))? 1 : 0;
}

int get_token(FILE *fp, struct token_sy *token) {
    int i, ch;
    char tmp[32];
    ch = fgetc(fp);
    while (is_blank(ch))
        ch = fgetc(fp);
    switch (get_char_type(ch)) {
    case LOWER_LETTER:
        token->sy = IDEN;
        token->val.strVal[0] = ch;
        for (i = 1; is_letter_or_digit(ch = fgetc(fp)); i++)
            token->val.strVal[i] = ch;
        ungetc(ch, fp);
        token->val.strVal[i] = '\0';
        //const, integer, char, var, array
        //of, if, then, else, do
        //while, for, to, downto, procedure
        //function, read, write, begin, end
        if (!strcmp(token->val.strVal, "const")) {
            token->sy = CONSTTK;
            return CONSTTK;
        } else if (!strcmp(token->val.strVal, "integer")) {
            token->sy = INTTK;
            return INTTK;
        } else if (!strcmp(token->val.strVal, "char")) {
            token->sy = CHARTK;
            return CHARTK;
        } else if (!strcmp(token->val.strVal, "var")) {
            token->sy = VARTK;
            return VARTK;
        } else if (!strcmp(token->val.strVal, "array")) {
            token->sy = ARRAYTK;
            return ARRAYTK;
        } else if (!strcmp(token->val.strVal, "of")) {//
            token->sy = OFTK;
            return OFTK;
        } else if (!strcmp(token->val.strVal, "then")) {
            token->sy = THENTK;
            return THENTK;
        } else if (!strcmp(token->val.strVal, "else")) {
            token->sy = ELSETK;
            return ELSETK;
        } else if (!strcmp(token->val.strVal, "do")) {
            token->sy = DOTK;
            return DOTK;
        } else if (!strcmp(token->val.strVal, "while")) {//
            token->sy = WHILETK;
            return WHILETK;
        } else if (!strcmp(token->val.strVal, "for")) {
            token->sy = FORTK;
            return FORTK;
        } else if (!strcmp(token->val.strVal, "to")) {
            token->sy = TOTK;
            return TOTK;
        } else if (!strcmp(token->val.strVal, "downto")) {
            token->sy = DOWNTOTK;
            return DOWNTOTK;
        } else if (!strcmp(token->val.strVal, "procedure")) {
            token->sy = PROCETK;
            return PROCETK;
        } else if (!strcmp(token->val.strVal, "function")) {//
            token->sy = FUNCTK;
            return FUNCTK;
        } else if (!strcmp(token->val.strVal, "read")) {
            token->sy = READTK;
            return READTK;
        } else if (!strcmp(token->val.strVal, "write")) {
            token->sy = WRITETK;
            return WRITETK;
        } else if (!strcmp(token->val.strVal, "begin")) {
            token->sy = BEGINTK;
            return BEGINTK;
        } else if (!strcmp(token->val.strVal, "end")) {
            token->sy = ENDTK;
            return ENDTK;
        }
        return IDEN;
    case UPPER_LETTER:
        token->sy = IDEN;
        token->val.strVal[0] = ch;
        for (i = 1; is_upper_letter(ch = fgetc(fp)); i++)
            token->val.strVal[i] = ch;
        ungetc(ch, fp);
        token->val.strVal[i] = '\0';
        return IDEN;
    case DIGIT:
        token->sy = INTCON;
        tmp[0] = ch;
        for (i = 1; get_char_type(ch = fgetc(fp)) == DIGIT; i++)
            tmp[i] = ch;
        ungetc(ch, fp);
        tmp[i] = '\0';
        token->val.intVal = atoi(tmp);
        return INTCON;
    case '+':
        token->sy = PLUS;
        strcpy(token->val.strVal, "+");
        return PLUS;
    case '-':
        token->sy = MINU;
        strcpy(token->val.strVal, "-");
        return MINU;
    case '*':
        token->sy = MULT;
        strcpy(token->val.strVal, "*");
        return MULT;
    case '/':
        token->sy = DIV;
        strcpy(token->val.strVal, "/");
        return DIV;
    case '(':
        token->sy = LPARENT;
        strcpy(token->val.strVal, "(");
        return LPARENT;
    case ')':
        token->sy = RPARENT;
        strcpy(token->val.strVal, ")");
        return RPARENT;
    case ',':
        token->sy = COMMA;
        strcpy(token->val.strVal, ",");
        return COMMA;
    case ';':
        token->sy = SEMICN;
        strcpy( token->val.strVal, ";");
        return SEMICN;
    case ':':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = ASSIGN;
            strcpy(token->val.strVal, ":=");
            return ASSIGN;
        }
        ungetc(ch, fp);
        token->sy = COLON;
        strcpy(token->val.strVal, ":");
        return COLON;
    case '\'':
        ch = fgetc(fp);
        if ('0'<= ch && ch <= '9') {
            char ch2 = fgetc(fp);
            if (ch2 == '\'') {
                token->sy = INTCON;
                token->val.intVal = ch;
                return INTCON;
            }
        } else if ('a'<= ch && ch <= 'z') {
            char ch2 = fgetc(fp);
            if (ch2 == '\'') {
                token->sy = CHARCON;
                token->val.intVal = ch;
                return CHARCON;
            }
        } else
            return -1;
    case '"':
        for (i = 0; is_str_val(ch = fgetc(fp)); i++)
            token->val.strVal[i] = ch;
        if (ch == '"') {
            token->sy = STRCON;
            token->val.strVal[i] = '\0';
            return STRCON;
        } else {
			// '"' should appear in pairs
            return -1;
        }
    case '<':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = LEQ;
            strcpy(token->val.strVal, "<=");
            return LEQ;
        } else if (ch == '>') {
            token->sy = NEQ;
            strcpy(token->val.strVal, "<>");
            return NEQ;
        }
        ungetc(ch, fp);
        token->sy = LSS;
        strcpy(token->val.strVal, "<");
        return LSS;
    case '>':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = GEQ;
            strcpy(token->val.strVal, ">=");
            return GEQ;
        }
        ungetc(ch, fp);
        token->sy = GRE;
        strcpy(token->val.strVal, ">");
        return GRE;
    case '=':
        token->sy = EQL;
        strcpy(token->val.strVal, "=");
        return EQL;
    case '[':
        token->sy = LBRACK;
        strcpy(token->val.strVal, "[");
        return LBRACK;
    case ']':
        token->sy = RBRACK;
        strcpy(token->val.strVal, "]");
        return RBRACK;
    case '.':
        token->sy = PERIOD;
        strcpy(token->val.strVal, ".");
        return PERIOD;
    case EOF:
        return 0;
    default:
        return -1;
    }
}

int analyse_and_print(FILE *in, FILE *out) {
    struct token_sy tmp;
    int token_count = 0;
    init_map_sy_string();
    while (get_token(in, &tmp) != 0) {
        fprintf(out, "%d ", ++token_count);
        fprintf(out, "%s ", map_sy_string[tmp.sy]);
        if (tmp.sy == INTCON) fprintf(out, "%d\n", tmp.val.intVal);
        else fprintf(out, "%s\n", tmp.val.strVal);
    }
    return 0;
}

int init_map_sy_string() {
    map_sy_string[11] = "IDEN";
    map_sy_string[21] = "INTCON";
    map_sy_string[31] = "REALCON";
    map_sy_string[41] = "CHARCON";
    map_sy_string[51] = "STRCON";
    map_sy_string[61] = "CONSTTK";
    map_sy_string[71] = "INTTK";
    map_sy_string[81] = "FLOATTK";
    map_sy_string[91] = "CHARTK";
    map_sy_string[101] = "VARTK";
    map_sy_string[111] = "ARRAYTK";
    map_sy_string[121] = "OFTK";

    map_sy_string[12] = "REPTTK";
    map_sy_string[22] = "UNLTK";
    map_sy_string[32] = "IFTK";
    map_sy_string[42] = "THENTK";
    map_sy_string[52] = "ELSETK";
    map_sy_string[62] = "DOTK";
    map_sy_string[72] = "WHILETK";
    map_sy_string[82] = "SWITCHTK";
    map_sy_string[92] = "CASETK";
    map_sy_string[102] = "FORTK";
    map_sy_string[112] = "TOTK";
    map_sy_string[122] = "BYTK";

    map_sy_string[13] = "DOWNTOTK";
    map_sy_string[23] = "PROCETK";
    map_sy_string[33] = "FUNCTK";
    map_sy_string[43] = "READTK";
    map_sy_string[53] = "WRITETK";
    map_sy_string[63] = "CALLTK";
    map_sy_string[73] = "BEGINTK";
    map_sy_string[83] = "ENDTK";
    map_sy_string[93] = "ODDTK";
    map_sy_string[103] = "PLUS";
    map_sy_string[113] = "MINU";

    map_sy_string[14] = "MULT";
    map_sy_string[24] = "DIV";
    map_sy_string[34] = "LSS";
    map_sy_string[44] = "LEQ";
    map_sy_string[54] = "GRE";
    map_sy_string[64] = "GEQ";
    map_sy_string[74] = "EQL";
    map_sy_string[84] = "NEQ";
    map_sy_string[94] = "ASSIGN";
    map_sy_string[104] = "SEMICN";
    map_sy_string[114] = "COMMA";

    map_sy_string[15] = "PERIOD";
    map_sy_string[25] = "COLON";
    map_sy_string[35] = "QMARK";
    map_sy_string[45] = "DQMARK";
    map_sy_string[55] = "LPARENT";
    map_sy_string[65] = "RPARENT";
    map_sy_string[75] = "LBRACK";
    map_sy_string[85] = "RBRACK";
    map_sy_string[95] = "LBRACE";
    map_sy_string[105] = "RBRACE";
    return 0;
}

