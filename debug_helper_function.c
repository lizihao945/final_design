#include "debug_helper_function.h"
struct token_sy token_history[120];
int idx = 0;
void get_token_with_history() {
    get_token(in, &token);
    token_history[idx++] = token;
}

void describe_token(struct token_sy token) {
    if (token.sy == INTCON) printf("%d\t", token.val.intVal);
    else printf("%s\t", token.val.strVal);
}

void print_action(char * const str, int * const vals) {
    printf("%s\t%d\t%d\t%d\n", str, vals[0], vals[1], vals[2]);
}

void describe_token_history(struct token_sy const *token_history, int st, int en) {
    int i;
    //since idx is ++ed every time a token is read,
    //the real index of last token should always - 1
    //and because of the pre-getting of token,
    //the end index should always - 1
    for(i = st - 1; i <= en - 2; i++)
        describe_token(token_history[i]);
    printf("\n");
}

void print_error(const char x[]) {
    printf("Error\t%s!\n", x);
//    printf("Error\t%s!\t Next token:\t%s\t", x, map_sy_string[token.sy]);
//    if (token.sy == INTCON)
//        printf("%d\n", token.val.intVal);
//    else
//        printf("%s\n", token.val.strVal);
}

void print_verbose(const char x[]) {
    printf("Verbose\t%s!\n", x);
//    printf("Verbose\t%s.\t Next token:\t%s\t", x, map_sy_string[token.sy]);
//    if (token.sy == INTCON)
//        printf("%d\n", token.val.intVal);
//    else
//        printf("%s\n", token.val.strVal);
}
