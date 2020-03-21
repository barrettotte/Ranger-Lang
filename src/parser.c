// Build syntax tree from tokens retrieved from lexer

#include "rangerlang.h"


static TokenType currentToken;

static TreeNode *parse_Block();
static TreeNode *parse_Statement();
static TreeNode *parse_IfStmt();
static TreeNode *parse_DowStmt();
static TreeNode *parse_AssignStmt();
static TreeNode *parse_ReadStmt();
static TreeNode *parse_WriteStmt();

static TreeNode *parse_Expression();
static TreeNode *parse_SimpleExpr();
static TreeNode *parse_Term();
static TreeNode *parse_Factor();


// Generic syntax error handler
static void syntaxError(const char *message){
    fprintf(g_Listing, "\n>>> ");
    fprintf(g_Listing, "Syntax error at line %d: %s", g_Lineno, message);
    g_Error = TRUE;
}


// Match current token with expected, otherwise syntax error thrown
static void matchToken(TokenType expected){
    if(currentToken == expected){
        currentToken = getToken();
    } else{
        syntaxError("unexpected token -> ");
        printToken(currentToken, g_Lexeme);
        fprintf(g_Listing, "        ");
        fprintf(g_Listing, "expected ");
        printToken(expected, " ");
    }
}


// Parse a block of statements
TreeNode *parse_Block(){
    TreeNode *t = parse_Statement();
    TreeNode *p = t;

    while( (currentToken != T_ENDFILE) 
        && (currentToken != T_ENDIF) && (currentToken != T_ELSE)    
        && (currentToken != T_ENDDO)){

        TreeNode *q;
        q = parse_Statement();

        if(q != NULL){
            if(t == NULL){
                t = p = q;
            } else{
                p->sibling = q;
                p = q;
            }
        }
    }
    return t;
}


// Parse a statement, router to other statement parser functions
TreeNode *parse_Statement(){
    TreeNode *t = NULL;

    switch(currentToken){
        case T_IF:         t = parse_IfStmt();     break;
        case T_DOW:         t = parse_DowStmt();   break;
        case T_IDENTIFIER: t = parse_AssignStmt(); break;
        case T_READ:       t = parse_ReadStmt();   break;
        case T_WRITE:      t = parse_WriteStmt();  break;
        default:
            syntaxError("unexpected token -> ");
            printToken(currentToken, g_Lexeme);
            currentToken = getToken();
            break; 
    }
    if(!g_Error){
        matchToken(T_SEMICOLON);
    }
    return t;
}


// Parse an if statement
TreeNode *parse_IfStmt(){
    TreeNode *t = newStmtNode(SK_IF);

    matchToken(T_IF);
    if(t != NULL){
        t->children[0] = parse_Expression(); // if conditional
    }
    matchToken(T_SEMICOLON);

    if(t != NULL){
        t->children[1] = parse_Block(); // if block
    }
    if(currentToken == T_ELSE){
        matchToken(T_ELSE);
        matchToken(T_SEMICOLON);
        if(t != NULL){
            t->children[2] = parse_Block(); // else block
        }
    }
    matchToken(T_ENDIF);
    return t;
}


// Parse a do while statement
TreeNode *parse_DowStmt(){
    TreeNode *t = newStmtNode(SK_DOW);

    matchToken(T_DOW);
    if(t != NULL){
        t->children[0] = parse_Expression(); // dow conditional
    }
    matchToken(T_SEMICOLON);

    if(t != NULL){
        t->children[1] = parse_Block();
    }
    matchToken(T_ENDDO);
    return t;
}


// Parse assignment statement
TreeNode *parse_AssignStmt(){
    TreeNode *t = newStmtNode(SK_ASSIGN);

    if((t != NULL) && (currentToken == T_IDENTIFIER)){
        t->attr.name = copyString(g_Lexeme);
    }
    matchToken(T_IDENTIFIER);
    matchToken(T_ASSIGN);
    if(t != NULL){
        t->children[0] = parse_Expression();
    }
    return t;
}


// Parse read statement
TreeNode *parse_ReadStmt(){
    TreeNode *t = newStmtNode(SK_READ);
    
    matchToken(T_READ);
    if((t != NULL) && (currentToken == T_IDENTIFIER)){
        t->attr.name = copyString(g_Lexeme);
    }
    matchToken(T_IDENTIFIER);
    return t;
}


// Parse write statement
TreeNode *parse_WriteStmt(){
    TreeNode  *t = newStmtNode(SK_WRITE);

    matchToken(T_WRITE);
    if(t != NULL){
        t->children[0] = parse_Expression();
    }
    return t;
}

// TODO: switch to pratt parsing

// Parse an expression
TreeNode *parse_Expression(){   
    TreeNode *t = parse_SimpleExpr();

    if((currentToken == T_LT) || (currentToken == T_GT) || (currentToken == T_EQ)){
        TreeNode *p = newExprNode(EK_OP);

        if(p != NULL){
            p->children[0] = t;
            p->attr.op = currentToken;
            t = p;
        }
        matchToken(currentToken);
        if(t != NULL){
            t->children[1] = parse_SimpleExpr();
        }
    }
    return t;
}


// Parse a simple expression (additive)
TreeNode *parse_SimpleExpr(){
    TreeNode *t = parse_Term();

    while((currentToken == T_PLUS) || (currentToken == T_MINUS)){
        TreeNode *p = newExprNode(EK_OP);

        if(p != NULL){
            p->children[0] = t;
            p->attr.op = currentToken;
            t = p;
            matchToken(currentToken);
            t->children[1] = parse_Term();
        }
    }
    return t;
}


// Parse a term (multiplicative)
TreeNode *parse_Term(){
    TreeNode *t = parse_Factor();

    while((currentToken == T_STAR) || (currentToken == T_SLASH)){
        TreeNode *p = newExprNode(EK_OP);

        if(p != NULL){
            p->children[0] = t;
            p->attr.op = currentToken;
            t = p;
            matchToken(currentToken);
            p->children[1] = parse_Factor();
        }
    }
    return t;
}


// Parse a factor
TreeNode *parse_Factor(){
    TreeNode *t = NULL;

    switch(currentToken){
        case T_NUMBER:
            t = newExprNode(EK_LITERAL);
            if((t != NULL) && (currentToken == T_NUMBER)){
                t->attr.val = atoi(g_Lexeme);
            }
            matchToken(T_NUMBER);
            break;
        case T_IDENTIFIER:
            t = newExprNode(EK_IDENTIFIER);
            if((t != NULL) && (currentToken == T_IDENTIFIER)){
                t->attr.name = copyString(g_Lexeme);
            }
            matchToken(T_IDENTIFIER);
            break;
        case T_LPAREN:
            matchToken(T_LPAREN);
            t = parse_Expression();
            matchToken(T_RPAREN);
            break;
        default:
            syntaxError("unexpected token -> ");
            printToken(currentToken, g_Lexeme);
            currentToken = getToken();
            break;
    }
    return t;
}


// Build syntax tree of source program
TreeNode *parse(){
    TreeNode *t;

    currentToken = getToken();
    t = parse_Block();
    if(currentToken != T_ENDFILE){
        syntaxError("unexpected end of file\n");
    }
    return t;
}
