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


/* Length/Size Definitions */
#define MAXKEYWORDS    7    // number of keywords
#define MAXTREECHILD   3    // number of children in tree
#define MAXTOKENLEN   40    // maximum token size
#define MAXSYMTBLDS  256    // maximum size of symbol table's data structure

/* Compile Listing Flags */
#define DEBUG_LEXER  FALSE  // include debug output for lexer
#define DEBUG_PARSER FALSE  // include debug output for parser
#define PRINT_TREE   TRUE   // include syntax tree in listing
#define PRINT_STDOUT FALSE  // print compile listing to STDOUT


/*********************************************************/
/*                     Global Variables                  */
/*********************************************************/

int g_Lineno;                  // source line number

FILE *g_Source;                // file pointer to source program
FILE *g_Listing;               // file pointer to compile listing
FILE *g_Target;                // file pointer to target program

int g_Error;                   // track if error occurs during compilation
char g_Lexeme[MAXTOKENLEN+1];  // lexeme of identifier or keyword



/*********************************************************/
/*                    Enum Definitions                   */
/*********************************************************/

typedef enum{
    T_ENDFILE, T_ERROR,
    T_IF, T_ELSE, T_ENDIF, T_DOW, T_ENDDO, 
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
TokenType getToken();                                  // return next token from source program

/* Parser */
TreeNode *parse();                                     // return built syntax tree of source program

/* Analyzer */
void buildSymbolTable(TreeNode *t);                    // build symbol table from syntax tree (preorder traversal)
void checkType(TreeNode *t);                           // type check syntax tree node (postorder traversal)

/* Syntax Tree */
TreeNode *newStmtNode(StmtKind sk);                    // create new statement node
TreeNode *newExprNode(ExprKind ek);                    // create new expression node

/* Symbol Table */
void insertSymbol(char *n, int ln, int ml);            // insert new symbol into symbol table
int  lookupSymbol(char *n);                            // lookup symbol from symbol table

/* Printing */
void printToken(TokenType tt, const char *c);          // print token and lexeme to compile listing
void printTree(TreeNode *r);                           // print tree to compile listing
void fprintf_if(FILE *f, const char *m, const int c);  // fprintf to file only if cond is true
void printFile(const char *fp);                        // print file contents to STDOUT
void printSymbolTable(FILE *f);                        // print formatted symbol table to file

/* Misc */
char *copyString(char *s);                             // make new copy of string s



#endif