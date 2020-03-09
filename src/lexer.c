#include "ranger-lang.h"


// Find char index in string, return -1 if not found
static int find(char *s, int c){
    char *p;
    p = strchr(s, c);
    return (p ? p - s : -1);
}


// Compare string with possible keywords
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


// Get next character from input file
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

// Reinsert unwanted character
static void reinsert(int c){
    g_Reinsert = c;
}

// Get character, ignoring unwanted characters (whitespace, newline)
static int skip(){
    int c = next();
    while(' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    }
    return c;
}

// Lex an integer literal
static int lex_Int(int c){
    int k, val = 0;
    while((k = find("0123456789", c)) >= 0){
        val = val * 10 + k;
        c = next();
    }
    reinsert(c);
    return val;
}

// Lex an identifer, store in buffer parm, return length of identifier
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
    reinsert(c);
    buff[i] = '\0';
    return i;
}

// Lex and return next token found from input
// Return 1 if valid token, 0 no tokens left
int lex(Token *t){
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
                t->intvalue  = lex_Int(c);
                t->token = T_INTLIT;
                break;
            } else if(isalpha(c) || '_' == c){
                lex_Identifier(c, g_Text, TEXT_LEN);
                if((tokenType = cmpKeywords(g_Text))){
                    t->token = tokenType;
                    break;
                }
                // Unrecognized keyword
                printf("Unrecognized symbol %s on line %d\n", g_Text, g_Line);
                exit(1);
            }
            printf("Invalid character %c on line %d", c, g_Line);
            exit(1);
    }
    return 1;
}