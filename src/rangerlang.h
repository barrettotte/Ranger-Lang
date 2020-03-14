/*********************************************************/
/*                      RangerLang                       */
/*                  Barrett Otte 2020                    */
/*                                                       */
/*********************************************************/

#ifndef RANGERLANG_H
#define RANGERLANG_H


#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*********************************************************/
/*                      Definitions                      */
/*********************************************************/

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


#define MAXKEYWORDS   8     // number of keywords
#define MAXTREECHILD  3     // number of children in tree
#define MAXTOKENLEN  40     // maximum token size



/*********************************************************/
/*                     Global Variables                  */
/*********************************************************/

int g_Lineno;                  // source line number

FILE* g_Source;                // file pointer to source program
FILE* g_Listing;               // file pointer to compile listing
FILE* g_Target;                // file pointer to target program

int g_Error;                   // track if error occurs during compilation
char g_Lexeme[MAXTOKENLEN+1];  // lexeme of identifier or keyword


/*********************************************************/
/*                    Enum Definitions                   */
/*********************************************************/

typedef enum{
    T_ENDFILE, T_ERROR,
    T_IF, T_THEN, T_ELSE, T_ENDIF, T_DOW, T_ENDDO, 
    T_READ, T_WRITE,
    T_IDENTIFIER, T_NUMBER,
    T_ASSIGN, 
    T_EQ, T_LT, T_GT,
    T_PLUS, T_MINUS, T_STAR, T_SLASH,
    T_LPAREN, T_RPAREN, 
    T_SEMICOLON
} TokenType;

typedef enum{
    NK_Stmt, NK_Expr
} NodeKind;

typedef enum{
    SK_IF, SK_DOW, 
    SK_ASSIGN,
    SK_READ, SK_WRITE
} StmtKind;

typedef enum{
    EK_OP, EK_LITERAL, EK_IDENTIFIER
} ExprKind;

typedef enum{
    ET_VOID, ET_INTEGER, ET_BOOLEAN
} ExprType;



/*********************************************************/
/*                 Struct Definitions                    */
/*********************************************************/

typedef struct treeNode{
    struct treeNode *children[MAXTREECHILD];
    struct treeNode *sibling;
    int lineno;
    NodeKind nodeKind;
    union{
        StmtKind stmt;
        ExprKind expr;
    } kind;
    union{
        TokenType op;
        int val;
        char *name;
    } attr;
    ExprType type;
} TreeNode; // nodes for the syntax tree



/*********************************************************/
/*                 Function Prototypes                   */
/*********************************************************/

/* Lexer */
TokenType getToken();                         // returns next token from source program

/* Parser */
TreeNode *parse();                            // returns built syntax tree of source program

/* Syntax Tree */
TreeNode *newStmtNode(StmtKind sk);           // create new statement node
TreeNode *newExprNode(ExprKind ek);           // create new expression node

/* Printing */
void printToken(TokenType tt, const char *c); // print token and lexeme to compile listing
void printTree(TreeNode *root);               // print tree to compile listing

/* Misc */
char *copyString(char *s);                    // make new copy of string s



#endif