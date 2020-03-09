#ifndef __RANGERLANG_H__
#define __RANGERLANG_H__

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SYMTBL_LEN  1024    // number of symbol table records
#define TEXT_LEN    512     // max length of current lexed string


typedef enum{
    T_EOF, 
    T_PLUS, T_MINUS, T_STAR, T_SLASH, 
    T_INTLITERAL,
    T_IDENTIFIER,
    T_INT,
    T_SEMICOLON, T_EQUALS,
    T_PRINT
} TokenType;

typedef enum{
    A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE,
    A_ASSIGN,
    A_LVIDENT, A_IDENTIFIER, // lvalue(symbol identifier), rvalue(symbol value)
    A_INTLITERAL
} NodeType;


typedef struct Token{
    int token;
    int intvalue;
} Token_t;

typedef struct AstNode{
    int op;
    struct AstNode *left;
    struct AstNode *right;
    union{
        int intvalue;   // literal
        int id;         // identifier
    } v;
} AstNode_t;

typedef struct SymbolRecord{
    char *name;     // symbol name
} SymbolRecord_t;


/* Global Variables */
SymbolRecord_t g_Symtbl[SYMTBL_LEN];    // symbol table
int g_NextFreeSymbol;                   // index of next free symbol record

int g_Line;                             // current line number
int g_Reinsert;                         // reinsert unwanted character to input stream

Token_t g_Token;                        // current token
char g_Text[TEXT_LEN + 1];              // current lexed text buffer

FILE *g_Source;                         // pointer to source program file
FILE *g_Target;                         // pointer to target program file
FILE *g_Listing;                        // pointer to compile listing output file


/* AST */
AstNode_t *newAstNode(int op, AstNode_t *left, AstNode_t *right, int intvalue);
AstNode_t *newAstLeaf(int op, int intvalue);
AstNode_t *newAstUnary(int op, AstNode_t *left, int intvalue);


/* Symbol Table */
int findSymbol(char *s);
int addSymbol(char *name);


/* Lexer */
int lex(Token_t *t);


/* Code Generator Interface */
int  gen_AST(AstNode_t *n, int reg);
void gen_Preamble();
void gen_Postamble();
void gen_ResetRegisters();
void gen_PrintInt(int reg);
void gen_NewSymbol(char *s);


/* CPU specific code generation   */
/* x86 64-bit assembly generation */
void cg_ResetRegisters();
void cg_Preamble();
void cg_Postamble();
int cg_Add(int r1, int r2);
int cg_Sub(int r1, int r2);
int cg_Mul(int r1, int r2);
int cg_Div(int r1, int r2);
int cg_LoadInt(int value);
void cg_PrintInt(int r);

int cg_LoadSymbol(char *identifier);
void cg_NewSymbol(char *symbol);
int cg_StoreSymbol(int r, char *identifier);


/* Parser */
AstNode_t *parse_BinaryExpr(int ptp);
void parse_Statements();


/* Misc */
char *strdup(const char *str);

void match(int t, char *expected);
void match_Semicolon();
void match_Identifier();

void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);


#endif