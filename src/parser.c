#include "ranger-lang.h"


// Operator precedences
static int opPrec[] = {0, 10, 10, 20, 20, 0};


// Parse primary factor and return AST node
static AstNode *parsePrimary(){
    AstNode *n;

    switch(g_Token.token){
        case T_INTLIT:
            n = newAstLeaf(A_INTLIT, g_Token.intvalue);
            lex(&g_Token);
            return n;
        default:
            fprintf(stderr, "Syntax error on line %d\n", g_Line);
            exit(1);
    }
}

// Convert token to AST operator
int arithop(int t){
    switch(t){
        case T_PLUS:    return A_ADD;
        case T_MINUS:   return A_SUBTRACT;
        case T_STAR:    return A_MULTIPLY;
        case T_SLASH:   return A_DIVIDE;
    }
    fprintf(stderr, "Unknown token '%c' on line %d\n", t, g_Line);
    exit(1);
}

// Return operator precedence of binary expression
static int opPrecedence(int tokenType){
    int prec = opPrec[tokenType];
    if(prec == 0){
        fprintf(stderr, "Syntax error on line %d, token %d, value %d\n", g_Line, g_Token.token, g_Token.intvalue);
        exit(1);
    }
    return prec;
}

// Return AST with binary operator as root node
AstNode *parse_BinExpr(int prevPrec){
    AstNode *left, *right;
    int tokenType;

    left = parsePrimary();
    tokenType = g_Token.token;

    if(tokenType == T_SEMI){
        return left;
    }

    while(opPrecedence(tokenType) > prevPrec){
        lex(&g_Token);
        right = parse_BinExpr(opPrec[tokenType]);
        left = newAstNode(arithop(tokenType), left, right, 0);
        
        tokenType = g_Token.token;
        if(tokenType == T_SEMI){
            return left;
        }
    }
    return left;
}

// Parse statements
void parse_Statements(){
    AstNode *ast;
    int reg;

    while(1){
        match(T_PRINT, "print");

        ast = parse_BinExpr(0);
        reg = gen_Ast(ast);
        gen_Printint(reg);
        gen_Reset();

        match_semi();
        if(g_Token.token == T_EOF){
            return;
        }
    }
}