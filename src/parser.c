#include "ranger-lang.h"


// Parse primary factor and return AST node
static AstNode_t *parse_PrimFactor(){
    AstNode_t *n;

    switch (g_Token.token){
        case T_INTLIT:
            n = newAstLeaf(A_INTLIT, g_Token.intvalue);
            lex(&g_Token);
            return n;
        default:
            fprintf(stderr, "syntax error on line %d, token %d\n", g_Line,
	        g_Token.token);
            exit(1);
    }
}

// Convert binary operator into AST operator
static int convertOperator(int tokenType){
    switch(tokenType){
        case T_PLUS:    return A_ADD;
        case T_MINUS:   return A_SUBTRACT;
        case T_STAR:    return A_MULTIPLY;
        case T_SLASH:   return A_DIVIDE;
    default:
      fprintf(stderr, "syntax error on line %d, token %d\n", g_Line, tokenType);
      exit(1);
  }
}

// Operator precedences 
static int precedences[] = {0, 10, 10, 20, 20, 0};

static int getPrecedence(int tokentype) {
    int prec = precedences[tokentype];
    if(prec == 0){
        fprintf(stderr, "syntax error on line %d, token %d\n", g_Line, tokentype);
        exit(1);
    }
  return prec;
}

// Return an AST tree whose root is a binary operator.
AstNode_t *parse_BinaryExpr(int prevPrec) {
    AstNode_t *left, *right;
    int tokenType;

    left = parse_PrimFactor();
    tokenType = g_Token.token;
    if(tokenType == T_SEMI){
        return left;
    }
    while(getPrecedence(tokenType) > prevPrec){
        lex(&g_Token);
        right = parse_BinaryExpr(precedences[tokenType]);
        left = newAstNode(convertOperator(tokenType), left, right, 0);

        tokenType = g_Token.token;
        if(tokenType == T_SEMI){
            return left;
        }
    }
    return left;
}

void parse_Statements(){
    AstNode_t *ast;
    int reg;
    
    while(1){
        match(T_PRINT, "print");
        ast = parse_BinaryExpr(0);
        reg = gen_AST(ast);
        gen_Printint(reg);
        gen_Freeregs();
        
        match_Semicolon();
        if(g_Token.token == T_EOF){
            return;
        }
    }
}
