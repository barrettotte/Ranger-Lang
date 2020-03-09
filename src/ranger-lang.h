#ifndef __RANGERLANG_H__
#define __RANGERLANG_H__

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define TEXT_LEN 512


typedef enum {
    T_EOF, T_SEMI,
    T_PLUS, T_MINUS, T_STAR, T_SLASH,
    T_INTLIT,
    T_PRINT
} TokenType;

typedef enum{
    A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT
} NodeType;


typedef struct token{
    int token;
    int intvalue;
} Token;

typedef struct astNode{
    int op;
    struct astNode *left;
    struct astNode *right;
    int intvalue;
} AstNode;


/* Globals */
int   g_Line;             // current line
int   g_Reinsert;         // reinsert character into input buffer
char  g_Text[TEXT_LEN+1]; // last identifier/keyword scanned
FILE  *g_Source;          // pointer to source program file
FILE  *g_Listing;         // pointer to compile listing output file
FILE  *g_Target;          // pointer to target program file
Token g_Token;            // current token


/* AST */
AstNode *newAstNode(int op, AstNode *left, AstNode *right, int intvalue);
AstNode *newAstLeaf(int op, int intvalue);
AstNode *newAstUnary(int op, AstNode *left, int intvalue);

/* Lexer */
int lex(Token *t);

/* Parser */
AstNode *parse_BinExpr(int prevPrec);
void parse_Statements();

/* Code Generation */
int gen_Ast(AstNode *n);
void gen_Preamble();
void gen_Postamble();
void gen_Reset();
void gen_Printint(int r);

/* x86 64-bit assembly code generation */
void cgasm_Reset();
void cgasm_Preamble();
void cgasm_Postamble();
int  cgasm_Load(int val);
int  cgasm_Add(int r1, int r2);
int  cgasm_Sub(int r1, int r2);
int  cgasm_Mul(int r1, int r2);
int  cgasm_Div(int r1, int r2);
void cgasm_Printint(int r);

/* Misc */
void match(int t, char *c);
void match_semi();

#endif