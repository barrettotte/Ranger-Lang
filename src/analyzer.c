// Analyze syntax tree to build symbol table and handle type checking

#include "rangerlang.h"


// track position in memory
static int memoryLocation = 0;


// Generic recursive tree traversal
//   applies preFunc in preorder and postFunc in postorder to syntax tree node t
static void traverse(TreeNode *t, void(*preFunc) (TreeNode *), void(*postFunc) (TreeNode *)){
    if(t != NULL){
        preFunc(t);
        for(int i = 0; i < MAXTREECHILD; i++){
            traverse(t->children[i], preFunc, postFunc);
        }
        postFunc(t);
        traverse(t->sibling, preFunc, postFunc);
    }
}

// Do nothing function, helper for generic traversal
static void nullFunc(TreeNode *t){
    return;
}


// Type error handler
static void typeError(TreeNode *t, char *msg){
    fprintf(g_Listing, "Type error at line %d: %s\n", t->lineno, msg);
    g_Error = TRUE;
}


// Helper for type errors, throw error if conditional is true
static void typeErrorIf(const int cond, TreeNode *t, char *msg){
    if(cond){
        typeError(t, msg);
    }
}


// Helper for inserting symbol from expression to symbol table 
static void insertExprNode(TreeNode *t){
    switch(t->kind.expr){
        case EK_IDENTIFIER:
            if(lookupSymbol(t->attr.name) == -1){
                insertSymbol(t->attr.name, t->lineno, memoryLocation++);
            } else{
                insertSymbol(t->attr.name, t->lineno, 0);
            }
            break;
        default:
            break;
    }
}


// Helper for inserting symbol from statement to symbol table
static void insertStmtNode(TreeNode *t){
    switch(t->kind.stmt){
        case SK_ASSIGN:
        case SK_READ:
            if(lookupSymbol(t->attr.name) == -1){
                // Not in symbol table, new symbol
                insertSymbol(t->attr.name, t->lineno, memoryLocation++);
            } else{
                // Exists in symbol table, add line number as reference
                insertSymbol(t->attr.name, t->lineno, 0);
            }
            break;
        default:
            break;
    }
}


// Insert identifier to symbol table
static void insertNode(TreeNode *t){
    switch(t->nodeKind){
        case NK_Stmt: insertStmtNode(t); break;
        case NK_Expr: insertExprNode(t); break;
        default:                         break;
    }
}


// Helper for checking statement node
static void checkStmtNode(TreeNode *t){
    switch(t->kind.stmt){
        case SK_IF:
            typeErrorIf(t->children[0]->type == ET_INTEGER, t->children[0], 
                "If conditional is not of type Boolean");
            break;
        case SK_ASSIGN:
            typeErrorIf(t->children[0]->type != ET_INTEGER, t->children[0], 
                "Assignment of non-integer value");
            break;
        case SK_WRITE:
            typeErrorIf(t->children[0]->type != ET_INTEGER, t->children[0], 
                "Write of non-integer value");
            break;
        case SK_DOW:
            typeErrorIf(t->children[0]->type == ET_INTEGER, t->children[1], 
                "Dow test is not of type Boolean");
            break;
        default:
            break;
    }
}


// Helper to identify a relational operator
static int isRelationOperator(const int op){
    return ((op == T_EQ) || (op == T_LT) || (op == T_GT));
}


// Helper for checking expression node
static void checkExprNode(TreeNode *t){
    switch(t->kind.expr){
        case EK_OP:
            typeErrorIf((t->children[0]->type != ET_INTEGER) || (t->children[1]->type != ET_INTEGER), t, 
                "Operator applied to non-integer data type");

            if(isRelationOperator(t->attr.op)){
                t->type = ET_BOOLEAN;
            } else{
                t->type = ET_INTEGER;
            }
            break;
        case EK_LITERAL:
        case EK_IDENTIFIER:
            t->type = ET_INTEGER;
            break;
        default:
            break;
    }
}


// Type check a single syntax tree node
static void checkNode(TreeNode *t){
    switch(t->nodeKind){
        case NK_Expr: checkExprNode(t); break;
        case NK_Stmt: checkStmtNode(t); break;
        default:                        break;
    }
}


// Build symbol table from syntax tree (preorder traversal)
void buildSymbolTable(TreeNode *syntaxTree){
    traverse(syntaxTree, insertNode, nullFunc);
    fprintf(g_Listing, "\nSymbol Table:\n\n");
    printSymbolTable(g_Listing);
}


// type check syntax tree node (postorder traversal)
void checkType(TreeNode *syntaxTree){
    traverse(syntaxTree, nullFunc, checkNode);
}
