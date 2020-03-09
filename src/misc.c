#include "ranger-lang.h"


char *strdup(const char *str){
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup){
        strcpy(dup, str);
    }
    return dup;
}


void match(int t, char *expected){
    if(g_Token.token == t){
        lex(&g_Token);
    } else{
        printf("%s expected on line %d\n", expected, g_Line);
        exit(1);
    }
}

// Match semicon and fetch next token
void match_Semicolon(){
    match(T_SEMICOLON, ";");
}

void match_Identifier(){
    match(T_IDENTIFIER, "identifier");
}


void fatal(char *s){
    fprintf(stderr, "%s on line %d\n", s, g_Line);
    exit(1);
}

void fatals(char *s1, char *s2){
    fprintf(stderr, "%s:%s on line %d\n", s1, s2, g_Line);
    exit(1);
}

void fatald(char *s, int d){
    fprintf(stderr, "%s:%d on line %d\n", s, d, g_Line);
    exit(1);
}

void fatalc(char *s, int c){
    fprintf(stderr, "%s:%c on line %d\n", s, c, g_Line);
    exit(1);
}
