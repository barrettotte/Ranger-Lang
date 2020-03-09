#include "ranger-lang.h"

// assert current token is t and fetch next token
void match(int t, char *expected){
    if(g_Token.token == t){
        lex(&g_Token);
    } else{
        printf("%s expected on line %d\n", expected, g_Line);
        exit(1);
    }
}

// Match semi colon and fetch next token
void match_semi(){
    match(T_SEMI, ";");
}
