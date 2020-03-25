#include "rangerlang.h"
#include "gen_rangervm.h"


// Memory offset for temps stored toward top of memory
static int tmpOffset = 0;

// Prototype to recursive code generator
static void generate(TreeNode *tree);


// Generate code for statements
static void generateStmt(TreeNode *tree){
    TreeNode *p1, *p2, *p3;
    int saveAddr1=0, saveAddr2=0, currAddr=0, addr=0;
    switch(tree->kind.stmt){
        case SK_IF:
            gen_Comment("IF");
            p1 = tree->children[0];
            p2 = tree->children[1];
            p3 = tree->children[2];

            generate(p1);
            saveAddr1 = gen_Skip(1);
            generate(p2);
            saveAddr2 = gen_Skip(1);
            currAddr = gen_Skip(0);
            gen_Backtrack(saveAddr1);
            gen_InsMemAbs("JEQ", AC, currAddr, "if: jump to else");
            gen_Restore();

            generate(p3);
            currAddr = gen_Skip(0);
            gen_Backtrack(saveAddr2);
            gen_InsMemAbs("LDA", PC, currAddr, "jump to end");
            gen_Restore();
            gen_Comment("ENDIF");
            break;
        case SK_DOW:
            gen_Comment("DOW");
            p1 = tree->children[1];
            p2 = tree->children[0];
            saveAddr1 = gen_Skip(0);
            
            generate(p1); // body
            generate(p2); // cond
            
            gen_InsMemAbs("JEQ", AC, saveAddr1, "dow: jump back to start");
            gen_Comment("ENDDO");
            break;
        case SK_ASSIGN:
            generate(tree->children[0]);
            addr = lookupSymbol(tree->attr.name);
            gen_InsMem("STM", AC, addr, GP, "assign: store value");
            break;
        case SK_READ:
            gen_InsReg("IN", AC, 0, 0, "read integer value");
            addr = lookupSymbol(tree->attr.name);
            gen_InsMem("STM", AC, addr, GP, "read: store value");
            break;
        case SK_WRITE:
            generate(tree->children[0]);
            gen_InsReg("OUT", AC, 0, 0, "write AC");
            break;
        default:
            break;
    }
}


// Helper function to generate operator (expression)
static void generateOperator(TreeNode *tree){
    switch(tree->attr.op){
        case T_PLUS:   gen_InsReg("ADD", AC, AC2, AC, "operator +");  break;
        case T_MINUS:  gen_InsReg("SUB", AC, AC2, AC, "operator -");  break;
        case T_STAR:   gen_InsReg("MUL", AC, AC2, AC, "operator *");  break;
        case T_SLASH:  gen_InsReg("DIV", AC, AC2, AC, "operator /");  break;
        case T_LT:
            gen_InsReg("SUB", AC, AC, AC2, "operator <");
            gen_InsMem("JLT", AC, 2, PC, "branch if true");
            gen_InsMem("LDI", AC, 0, AC, "it was false");
            gen_InsMem("LDA", PC, 1, PC, "unconditioned jump");
            gen_InsMem("LDI", AC, 1, AC, "it was true");
            break;
        case T_GT:
            gen_InsReg("SUB", AC, AC2, AC, "operator >");
            gen_InsMem("JGT", AC, 2, PC, "branch if true");
            gen_InsMem("LDI", AC, 0, AC, "it was false");
            gen_InsMem("LDA", PC, 1, PC, "unconditioned jump");
            gen_InsMem("LDI", AC, 1, AC, "it was true");
            break;
        case T_EQ:
            gen_InsReg("SUB", AC, AC2, AC, "operator ==");
            gen_InsMem("JEQ", AC, 2, PC, "branch if true");
            gen_InsMem("LDI", AC, 0, AC, "it was false");
            gen_InsMem("LDA", PC, 1, PC, "unconditioned jump");
            gen_InsMem("LDI", AC, 1, AC, "it was true");
            break;
        default:
            gen_Comment("ERROR: unknown operator found");
            return;
        }
}


// Generate code for expressions
static void generateExpr(TreeNode *tree){
    int addr = 0;
    TreeNode *p1, *p2;
    
    switch(tree->kind.expr){
        case EK_LITERAL:
            gen_InsMem("LDI", AC, tree->attr.val, 0, "load constant");
            break;
        case EK_IDENTIFIER:
            addr = lookupSymbol(tree->attr.name);
            gen_InsMem("LDM", AC, addr, GP, "load identifier value");
            break;
        case EK_OP:
            p1 = tree->children[0];
            p2 = tree->children[1];

            generate(p1);
            gen_InsMem("STM", AC, tmpOffset--, MP, "operator: push left");
            generate(p2);
            gen_InsMem("LDM", AC2, ++tmpOffset, MP, "operator: load left");
            generateOperator(tree);
            break;
        default:
            break;
    }
}


// Recursively generate code
static void generate(TreeNode *tree){
    if(tree != NULL){
        switch(tree->nodeKind){
            case NK_Stmt:  generateStmt(tree);  break;
            case NK_Expr:  generateExpr(tree);  break;
            default:                            break;
        }
        generate(tree->sibling);
    }
}


// generate code from syntax tree traversal
void generateCode(TreeNode *syntaxTree, const char *fileName){
    char *s = malloc(strlen(fileName)+7);
    strcpy(s, "File: ");
    strcat(s, fileName);
    gen_Comment("RangerLang compilation to RangerVM Assembly");
    gen_Comment(s);
    gen_Comment(" ");
    
    gen_Comment("START PRELUDE:");
    gen_InsMem("LDM", MP, 0, AC, "load maxaddress from address 0");
    gen_InsMem("STM", AC, 0, AC, "clear address 0");
    gen_Comment("END PRELUDE");

    generate(syntaxTree);
    gen_Comment("END PROGRAM");
    gen_InsReg("HALT", 0, 0, 0, "");
}
