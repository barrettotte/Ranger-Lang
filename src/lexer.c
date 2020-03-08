#include "common.h"
#include "globals.h"

// Get next character from input file
static int next(){
    int c;

    if(g_putback){
        c = g_putback;
        g_putback = 0;
        return c;
    }
    c = fgetc(g_infile);
    if('\n' == c){
        g_line++;
    }
    return c;
}

// Reinsert unwanted character
static void reinsert(int c){
    g_putback = c;
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
static int lexint(int c){
    int k, val = 0;
    while((k = find("01234567890", c)) >= 0){
        val = val * 10 + k;
        c = next();
    }
    reinsert(c); // reinsert non-integer character
    return val;
}

// Lex and return next token found from input
// Return 1 if valid token, 0 no tokens left
int lex(struct token *t){
    int c = skip(); // skip whitespace
    switch(c){
        case EOF: 
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
        default:
            if(isdigit(c)){
                t->intvalue  = lexint(c);
                t->token = T_INTLIT;
                break;
            }
            printf("Invalid character %c on line %d", c, g_line);
            exit(1);
    }
    return 1;
}