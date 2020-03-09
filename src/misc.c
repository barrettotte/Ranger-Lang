#include "ranger-lang.h"


void match(int t, char *expected){
    if(g_Token.token == t){
        lex(&g_Token);
    } else{
        printf("%s expected on line %d\n", expected, g_Line);
        exit(1);
    }
}

// Match semicon and fetch next token
void match_Semicolon() {
    match(T_SEMI, ";");
}
