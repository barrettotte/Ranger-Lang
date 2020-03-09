#include "ranger-lang.h"


// Parse primary factor and return AST node
static AstNode_t *parse_Primary(){
    AstNode_t *n;
    int identifier;
    
    switch (g_Token.token){
        case T_INTLITERAL:
            n = newAstLeaf(A_INTLITERAL, g_Token.intvalue);
            break;
        case T_IDENTIFIER:
            identifier = findSymbol(g_Text);
            if(identifier == -1){
                fatals("Unknown variable", g_Text);
            }
            n = newAstLeaf(A_IDENTIFIER, identifier);
            break;
        default:
            fatald("Syntax error, token", g_Token.token);
            break;
    }
    lex(&g_Token);
    return n;
}

// Convert binary operator (token) to AST operator
static int convertOperator(int tokenType){
    switch(tokenType){
        case T_PLUS:    return A_ADD;
        case T_MINUS:   return A_SUBTRACT;
        case T_STAR:    return A_MULTIPLY;
        case T_SLASH:   return A_DIVIDE;
    }
    fprintf(stderr, "syntax error on line %d, token %d\n", g_Line, tokenType);
    exit(1);
}

// Operator precedence table  (Pratt Parsing)
// https://journal.stuffwithstuff.com/2011/03/19/pratt-parsers-expression-parsing-made-easy/
static int precedences[] = {
    0,      // EOF
    10, 10, // + -  (additive)
    20, 20, // * /  (multiplicative)
    0       // INTLITERAL
};

// Get precendence of operator
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

    left = parse_Primary();
    tokenType = g_Token.token;
    if(tokenType == T_SEMICOLON){
        return left;
    }
    while(getPrecedence(tokenType) > prevPrec){
        lex(&g_Token);
        right = parse_BinaryExpr(precedences[tokenType]);
        left = newAstNode(convertOperator(tokenType), left, right, 0);

        tokenType = g_Token.token;
        if(tokenType == T_SEMICOLON){
            return left;
        }
    }
    return left;
}

void parse_VarDeclaration(){
    match(T_INT, "int");

    match_Identifier();
    addSymbol(g_Text);
    gen_NewSymbol(g_Text);

    match_Semicolon();
}

void parse_PrintStatement(){
    AstNode_t *ast;
    int reg;
    
    match(T_PRINT, "print"); // match first token

    // parse and generate target code
    ast = parse_BinaryExpr(0);
    reg = gen_AST(ast, -1);
    gen_PrintInt(reg);
    gen_ResetRegisters();

    match_Semicolon();
}

void parse_AssignStatement(){
    AstNode_t *left, *right, *ast;
    int identifier;

    match_Identifier();
    if((identifier = findSymbol(g_Text)) == -1){
        fatals("Undeclared variable", g_Text);
    }

    right = newAstLeaf(A_LVIDENT, identifier);
    match(T_EQUALS, "=");
    left = parse_BinaryExpr(0);
    ast = newAstNode(A_ASSIGN, left, right, 0);

    // Generate target code
    gen_AST(ast, -1);
    gen_ResetRegisters();

    match_Semicolon();
}

void parse_Statements(){
    while(1){
        switch(g_Token.token){
            case T_PRINT:       parse_PrintStatement();             break;
            case T_IDENTIFIER:  parse_AssignStatement();            break;
            case T_INT:         parse_VarDeclaration();             break;
            case T_EOF:         return;
            default: fatald("Syntax error, token", g_Token.token);  break;
        }
        
    }
}
