/*
	Zihao Li
	1106111
	Nov. 1st, 2013
*/
#include "lexer.h"
struct token_st token;

int get_char_type(char ch) {
    if (ch >= 'a' && ch <= 'z') return LOWER_LETTER;
    if (ch >= 'A' && ch <= 'Z') return UPPER_LETTER;
    if (ch >= '0' && ch <= '9') return DIGIT;
    return ch;
}

int is_blank(char ch) {
	if (ch == '\n') line_num++;
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

int get_token(FILE *fp, struct token_st *token) {
    int i, ch;
    char tmp[32];
    ch = fgetc(fp);
    while (is_blank(ch))
		ch = fgetc(fp);
	// if end of file reached
	if (ch == -1)
		return -1;
    switch (get_char_type(ch)) {
    case LOWER_LETTER:
        token->sy = IDEN;
        token->val.str_val[0] = ch;
        for (i = 1; is_letter_or_digit(ch = fgetc(fp)); i++)
            token->val.str_val[i] = ch;
        ungetc(ch, fp);
        token->val.str_val[i] = '\0';
        //const, integer, char, var, array
        //of, if, then, else, do
        //while, for, to, downto, procedure
        //function, read, write, begin, end
        if (!strcmp(token->val.str_val, "const")) {
            token->sy = CONSTTK;
            return CONSTTK;
        } else if (!strcmp(token->val.str_val, "integer")) {
            token->sy = INTTK;
            return INTTK;
        } else if (!strcmp(token->val.str_val, "char")) {
            token->sy = CHARTK;
            return CHARTK;
        } else if (!strcmp(token->val.str_val, "var")) {
            token->sy = VARTK;
            return VARTK;
        } else if (!strcmp(token->val.str_val, "array")) {
            token->sy = ARRAYTK;
            return ARRAYTK;
        } else if (!strcmp(token->val.str_val, "of")) {
            token->sy = OFTK;
            return OFTK;
		} else if(!strcmp(token->val.str_val, "if")) {
			token->sy = IFTK;
			return IFTK;
        } else if (!strcmp(token->val.str_val, "then")) {
            token->sy = THENTK;
            return THENTK;
        } else if (!strcmp(token->val.str_val, "else")) {
            token->sy = ELSETK;
            return ELSETK;
        } else if (!strcmp(token->val.str_val, "do")) {
            token->sy = DOTK;
            return DOTK;
        } else if (!strcmp(token->val.str_val, "while")) {//
            token->sy = WHILETK;
            return WHILETK;
        } else if (!strcmp(token->val.str_val, "for")) {
            token->sy = FORTK;
            return FORTK;
        } else if (!strcmp(token->val.str_val, "to")) {
            token->sy = TOTK;
            return TOTK;
        } else if (!strcmp(token->val.str_val, "downto")) {
            token->sy = DOWNTOTK;
            return DOWNTOTK;
        } else if (!strcmp(token->val.str_val, "procedure")) {
            token->sy = PROCETK;
            return PROCETK;
        } else if (!strcmp(token->val.str_val, "function")) {//
            token->sy = FUNCTK;
            return FUNCTK;
        } else if (!strcmp(token->val.str_val, "read")) {
            token->sy = READTK;
            return READTK;
        } else if (!strcmp(token->val.str_val, "write")) {
            token->sy = WRITETK;
            return WRITETK;
        } else if (!strcmp(token->val.str_val, "begin")) {
            token->sy = BEGINTK;
            return BEGINTK;
        } else if (!strcmp(token->val.str_val, "end")) {
            token->sy = ENDTK;
            return ENDTK;
        }
        return IDEN;
    case UPPER_LETTER:
        token->sy = IDEN;
        token->val.str_val[0] = ch;
        for (i = 1; is_upper_letter(ch = fgetc(fp)); i++)
            token->val.str_val[i] = ch;
        ungetc(ch, fp);
        token->val.str_val[i] = '\0';
        return IDEN;
    case DIGIT:
        token->sy = INTCON;
        tmp[0] = ch;
        for (i = 1; get_char_type(ch = fgetc(fp)) == DIGIT; i++)
            tmp[i] = ch;
        ungetc(ch, fp);
        tmp[i] = '\0';
        token->val.int_val = atoi(tmp);
        return INTCON;
    case '+':
        token->sy = PLUS;
        strcpy(token->val.str_val, "+");
        return PLUS;
    case '-':
        token->sy = MINU;
        strcpy(token->val.str_val, "-");
        return MINU;
    case '*':
        token->sy = MULT;
        strcpy(token->val.str_val, "*");
        return MULT;
    case '/':
        token->sy = DIV;
        strcpy(token->val.str_val, "/");
        return DIV;
    case '(':
        token->sy = LPARENT;
        strcpy(token->val.str_val, "(");
        return LPARENT;
    case ')':
        token->sy = RPARENT;
        strcpy(token->val.str_val, ")");
        return RPARENT;
    case ',':
        token->sy = COMMA;
        strcpy(token->val.str_val, ",");
        return COMMA;
    case ';':
        token->sy = SEMICN;
        strcpy( token->val.str_val, ";");
        return SEMICN;
    case ':':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = ASSIGN;
            strcpy(token->val.str_val, ":=");
            return ASSIGN;
        }
        ungetc(ch, fp);
        token->sy = COLON;
        strcpy(token->val.str_val, ":");
        return COLON;
    case '\'':
        ch = fgetc(fp);
        if (('0'<= ch && ch <= '9') || ('a'<= ch && ch <= 'z') || ('A'<= ch && ch <= 'Z')) {
            char ch2 = fgetc(fp);
            if (ch2 == '\'') {
                token->sy = CHARCON;
                token->val.int_val = ch;
                return CHARCON;
			} else {
				eval_error(ERR_QMARK_MISSED, "");
				return -1;
			}
		} else {
            eval_error(ERR_DQMARK_MISSED, "");
			return -1;
		}
    case '"':
        for (i = 0; is_str_val(ch = fgetc(fp)); i++)
            token->val.str_val[i] = ch;
        if (ch == '"') {
            token->sy = STRCON;
            token->val.str_val[i] = '\0';
            return STRCON;
        } else {
			// '"' should appear in pairs
            return -1;
        }
    case '<':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = LEQ;
            strcpy(token->val.str_val, "<=");
            return LEQ;
        } else if (ch == '>') {
            token->sy = NEQ;
            strcpy(token->val.str_val, "<>");
            return NEQ;
        }
        ungetc(ch, fp);
        token->sy = LSS;
        strcpy(token->val.str_val, "<");
        return LSS;
    case '>':
        ch = fgetc(fp);
        if (ch == '=') {
            token->sy = GEQ;
            strcpy(token->val.str_val, ">=");
            return GEQ;
        }
        ungetc(ch, fp);
        token->sy = GRE;
        strcpy(token->val.str_val, ">");
        return GRE;
    case '=':
        token->sy = EQL;
        strcpy(token->val.str_val, "=");
        return EQL;
    case '[':
        token->sy = LBRACK;
        strcpy(token->val.str_val, "[");
        return LBRACK;
    case ']':
        token->sy = RBRACK;
        strcpy(token->val.str_val, "]");
        return RBRACK;
    case '.':
        token->sy = PERIOD;
        strcpy(token->val.str_val, ".");
        return PERIOD;
    case EOF:
        return 0;
    default:
        return -1;
    }
}

int analyse_and_print(FILE *in, FILE *out) {
    struct token_st tmp;
    int token_count = 0;
    while (get_token(in, &tmp) != 0) {
        fprintf(out, "%d ", ++token_count);
        fprintf(out, "%s ", map_sy_string[tmp.sy]);
        if (tmp.sy == INTCON) fprintf(out, "%d\n", tmp.val.int_val);
        else fprintf(out, "%s\n", tmp.val.str_val);
    }
    return 0;
}
