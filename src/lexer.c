#include "rangerlang.h"


#define LINEBUFFLEN 256     // length of source line buffer

// Lexer DFA states
typedef enum{
    LS_START, LS_INASSIGN, 
    LS_ENTERCOMMENT, LS_INCOMMENT, LS_EXITINGCOMMENT,
    LS_INNUMBER, LS_INIDENTIFIER, LS_DONE
} LexerState;


// Lookup table of keywords
static struct{
    char *str;
    TokenType token;
} keywords[MAXKEYWORDS] = {
    {"if", T_IF}, 
    {"then", T_THEN}, 
    {"else", T_ELSE}, 
    {"endif", T_ENDIF},
    {"dow", T_DOW},
    {"enddo", T_ENDDO},
    {"read", T_READ},
    {"write", T_WRITE}
};


// lexeme of identifier or keyword
char g_Lexeme[MAXTOKENLEN+1];

static char lineBuffer[LINEBUFFLEN]; // current line of source
static int linePos = 0;              // current position in line buffer
static int bufferSize = 0;           // current size of line buffer


// Get next non-whitespace character from line buffer
static char getNextChar(){
    if(!(linePos < bufferSize)){
        g_Lineno++;
        if(fgets(lineBuffer, LINEBUFFLEN-1, g_Source)){
            fprintf(g_Listing, "%4d: %s", g_Lineno, lineBuffer);
            bufferSize = strlen(lineBuffer);
            linePos = 0;
            return lineBuffer[linePos++];
        } else{
            return EOF;
        }
    }
    return lineBuffer[linePos++];
}


// Backtrack one character in line buffer
static void backtrack(){
    linePos--;
}


// Search for keyword and get its token, otherwise assumed identifier
static TokenType lookupKeyword(char *identifer){
    for(int i = 0; i < MAXKEYWORDS; i++){
        if(!strcmp(identifer, keywords[i].str)){
            return keywords[i].token;
        }
    }
    return T_IDENTIFIER;
}


// Return next token in source program
// Attempting to be a deterministic finite automata (DFA)
TokenType getToken(){
    int lexemeIndex = 0;
    TokenType currentToken;
    LexerState state = LS_START;
    int saveLexeme;

    while(state != LS_DONE){
        char c = getNextChar();
        saveLexeme = TRUE;

        switch(state){
            case LS_START:
                if(isdigit(c)){
                    state = LS_INNUMBER;
                } else if(isalpha(c)){
                    state = LS_INIDENTIFIER;
                } else if(c == ':'){
                    state = LS_INASSIGN;
                } else if((c == ' ') || (c == '\t') || (c == '\n')){
                    saveLexeme = FALSE;
                }
                else if(c == '/'){
                    saveLexeme = FALSE;
                    state = LS_ENTERCOMMENT;
                } else{
                    state = LS_DONE;
                    switch(c){
                        case EOF:
                            saveLexeme = FALSE;
                            currentToken = T_ENDFILE;
                            break;
                        case '/': currentToken = T_SLASH;     break;
                        case '=': currentToken = T_EQ;        break;
                        case '<': currentToken = T_LT;        break;
                        case '>': currentToken = T_GT;        break;
                        case '+': currentToken = T_PLUS;      break;
                        case '-': currentToken = T_MINUS;     break;
                        case '*': currentToken = T_STAR;      break;
                        case '(': currentToken = T_LPAREN;    break;
                        case ')': currentToken = T_RPAREN;    break;
                        case ';': currentToken = T_SEMICOLON; break;
                        default:  currentToken = T_ERROR;     break;
                    }
                }
                break;
            case LS_EXITINGCOMMENT:
                saveLexeme = FALSE;
                if(c == '/'){
                    state = LS_START;
                } else{
                    state = LS_INCOMMENT;
                }
                break;
            case LS_INCOMMENT:
                saveLexeme = FALSE;
                if(c == '*'){    
                    state = LS_EXITINGCOMMENT;
                }
                break;
            case LS_ENTERCOMMENT:
                if(c == '*'){
                    saveLexeme = FALSE;
                    state = LS_INCOMMENT;
                } else{
                    saveLexeme = TRUE;
                    state = LS_START;
                }
                break;
            case LS_INASSIGN:
                state = LS_DONE;
                if(c == '='){
                    currentToken = T_ASSIGN;
                } else{
                    backtrack();
                    saveLexeme = FALSE;
                    currentToken = T_ERROR;
                }
                break;
            case LS_INNUMBER:
                if(!isdigit(c)){
                    backtrack();
                    saveLexeme = FALSE;
                    state = LS_DONE;
                    currentToken = T_NUMBER;
                }
                break;
            case LS_INIDENTIFIER:
                if(!isalpha(c)){
                    backtrack();
                    saveLexeme = FALSE;
                    state = LS_DONE;
                    currentToken = T_IDENTIFIER;
                }
                break;
            case LS_DONE: 
            default:
                fprintf(g_Listing, "Lexical bug: state = %d\n", state);
                state = LS_DONE;
                currentToken = T_ERROR;
                break;
        }
        if((saveLexeme) && (lexemeIndex < MAXTOKENLEN)){
            g_Lexeme[lexemeIndex++] = c;
        }
        if(state == LS_DONE){
            g_Lexeme[lexemeIndex] = '\0';
            if(currentToken == T_IDENTIFIER){
                currentToken = lookupKeyword(g_Lexeme);
            }
        }
    }
    return currentToken;
}
