#include "ranger-lang.h"


// Find char index in string, return -1 if not found
static int find(char *s, int c){
    char *p;
    p = strchr(s, c);
    return p ? p - s : -1;
}

// Fetch next character from source file
static int next(){
    int c;

    if(g_Reinsert){
        c = g_Reinsert;
        g_Reinsert = 0;
        return c;
    }
    c = fgetc(g_Source);
    if('\n' == c){
        g_Line++;
    }
    return c;
}

// Reinsert unwanted character back into stream
static void reinsert(int c){
    g_Reinsert = c;
}

// Skip whitespace, return next 'interesting' character
static int skip(){
    int c;
    c = next();
    while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    }
    return c;
}

// Lex integer literal
static int lex_Integer(int c){
    int k, val = 0;
    while((k = find("0123456789", c)) >= 0){
        val = val * 10 + k;
        c = next();
    }
    reinsert(c); // reinsert non-integer character
    return val;
}

// Lex identifier, store in buff, and return identifier length
static int lex_Identifier(int c, char *buff, int lim){
    int i = 0;
    while(isalpha(c) || isdigit(c) || '_' == c){
        if(lim - 1 == i){
            printf("Identifier too long on line %d\n", g_Line);
            exit(1);
        } else if(i < lim - 1){
            buff[i++] = c;
        }
        c = next();
    }
    reinsert(c); // reinsert non-valid character
    buff[i] = '\0';
    return i;
}

// Compare string to possible keywords
static int cmpKeywords(char *s){
    switch(*s){
        case 'p':
            if(!strcmp(s, "print")){
                return T_PRINT;
            }
            break;
    }
    return 0;
}

// Lex next token. 1 if found, 0 if no tokens left
int lex(Token_t *t){
    int c, tokenType;
    c = skip(); // skip whitespace
    
    switch(c){
        case EOF:
            t->token = T_EOF;
            return 0;
        case '+':
            t->token = T_PLUS;
            break;
        case '-':
            t->token = T_MINUS;
            break;
        case '*':
            t->token = T_STAR;
            break;
        case '/':
            t->token = T_SLASH;
            break;
        case ';':
            t->token = T_SEMI;
            break;
        default:
            if(isdigit(c)){
                t->intvalue = lex_Integer(c);
                t->token = T_INTLIT;
                break;
            } else if(isalpha(c) || '_' == c){
                lex_Identifier(c, g_Text, TEXT_LEN);
                if((tokenType = cmpKeywords(g_Text))){
                    t->token = tokenType;
                    break;
                }
                printf("Unrecognised symbol %s on line %d\n", g_Text, g_Line);
                exit(1);
            }
            printf("Unrecognised character %c on line %d\n", c, g_Line);
            exit(1);
    }
    return 1;
}