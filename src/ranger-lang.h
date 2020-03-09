#ifndef __RANGERLANG_H__
#define __RANGERLANG_H__

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEXT_LEN 512

typedef enum{
    T_EOF, 
    T_PLUS, T_MINUS, T_STAR, T_SLASH, 
    T_INTLIT, 
    T_SEMI, T_PRINT
} TokenType;

typedef enum{
    A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT
} NodeType;

typedef struct Token{
    int token;
    int intvalue;
} Token_t;

typedef struct AstNode{
    int op;
    struct AstNode *left;
    struct AstNode *right;
    int intvalue;
} AstNode_t;


/* Globals */
int g_Line;                 // current line number
int g_Reinsert;             // reinsert unwanted character to input stream
FILE *g_Source;             // pointer to source program file
FILE *g_Target;             // pointer to target program file
FILE *g_Listing;            // pointer to compile listing output file
Token_t g_Token;            // current token
char g_Text[TEXT_LEN + 1];  // current lexed text


/* AST */
AstNode_t *newAstNode(int op, AstNode_t *left, AstNode_t *right, int intvalue);
AstNode_t *newAstLeaf(int op, int intvalue);
AstNode_t *newAstUnary(int op, AstNode_t *left, int intvalue);

/* Lexer */
int lex(Token_t *t);

/* Generic Code Generator */
int  gen_AST(AstNode_t *n);
void gen_Preamble();
void gen_Postamble();
void gen_Freeregs();
void gen_Printint(int reg);

/* CPU specific code generation   */
/* x86 64-bit assembly generation */
void resetRegisters();
void cgpreamble();
void cgpostamble();
int cgload(int value);
int cgadd(int r1, int r2);
int cgsub(int r1, int r2);
int cgmul(int r1, int r2);
int cgdiv(int r1, int r2);
void cgprintint(int r);

/* Parser */
AstNode_t *parse_BinaryExpr(int ptp);
void parse_Statements();

/* Misc */
void match(int t, char *expected);
void match_Semicolon();


#endif