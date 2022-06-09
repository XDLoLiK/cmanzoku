#include "tokenizer.h"

struct Tokenizer *Tokenizer_New(const char *fileName)
{
    struct Tokenizer *tok = calloc(1, sizeof (struct Tokenizer));
    if (tok == NULL) {
        return tok;
    }
    tok->file = fopen(fileName, "r");
    if (tok->file == NULL) {
        tok->errorCode = TOK_ERR_NotExistingFile;
        return tok;
    }

    size_t fileSize = GetFileSize(tok->file);
    tok->bufferStart = calloc(fileSize, sizeof (char));
    if (tok->bufferStart == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        return tok;
    }
    fread(tok->bufferStart, sizeof (char), fileSize, tok->file);
    tok->bufferCurrent = tok->bufferStart;
    tok->bufferEnd     = tok->bufferStart + fileSize;    
    tok->currentToken  = tok->bufferStart;
    tok->currentLine = tok->bufferStart;

    tok->errorCode  = TOK_ERR_NoError;
    tok->lineNumber = 1;
    return tok;
}

struct Token *Tokenizer_GetNextToken(struct Tokenizer *tok)
{
    Tokenizer_RecognizeStartOfData(tok);
    tok->currentToken = tok->bufferCurrent;

    if (tok->bufferCurrent >= tok->bufferEnd) {
        return Tokenizer_RecognizeEndOfData(tok);
    }

    if (IsDigit(*tok->bufferCurrent)) {
        return Tokenizer_RecognizeNumber(tok);
    }

    if (IsIdentifierStart(*tok->bufferCurrent)) {
        return Tokenizer_RecognizeIdentifier(tok);
    }

    if (IsOperator(*tok->bufferCurrent)) {
        return Tokenizer_RecognizeOperator(tok);
    }

    if (*tok->bufferCurrent == '\"') {
        return Tokenizer_RecognizeString(tok);
    }

    tok->errorCode = TOK_ERR_ImpossibleCharSequence;
    return NULL;
}

void Tokenizer_RecognizeStartOfData(struct Tokenizer *tok)
{
    while (tok->bufferCurrent < tok->bufferEnd) {
        if (IsSpace(*tok->bufferCurrent)) {
            if (*tok->bufferCurrent == '\n') {
                tok->lineNumber++;
                tok->currentLine = tok->bufferCurrent + 1;
            }
            tok->bufferCurrent++;
            continue;
        }
    
        if (*tok->bufferCurrent == '#') {
            Tokenizer_RecognizeCommentary(tok);
            continue;
        }
        break;
    }
    return;
}

void Tokenizer_RecognizeCommentary(struct Tokenizer *tok)
{
    if (*tok->bufferCurrent != '#') {
        tok->errorCode = TOK_ERR_ImpossibleState;
        return;
    }
    tok->bufferCurrent++;

    while (tok->bufferCurrent < tok->bufferEnd) {
        if (*tok->bufferCurrent == '\n') {
            tok->lineNumber++;
            tok->bufferCurrent++;
            tok->currentLine = tok->bufferCurrent;
            return;            
        }
        tok->bufferCurrent++;
    }
    tok->errorCode = TOK_ERR_AbruptEndOfComment;
    return;
}

struct Token *Tokenizer_RecognizeEndOfData(struct Tokenizer *tok)
{
    struct Token *token = calloc(1, sizeof (struct Token));
    if (token == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        return token;
    }
    token->type = TOKEN_TYPE_Endmarker;
    token->operator = TONEN_OP_NotAnOperator;
    return token;
}

struct Token *Tokenizer_RecognizeIdentifier(struct Tokenizer *tok)
{
    while (tok->bufferCurrent < tok->bufferEnd) {
        if (!IsIdentifierSym(*tok->bufferCurrent)) {
            return Tokenizer_NewIdentifier(tok, tok->currentToken, 
                                           tok->bufferCurrent - tok->currentToken);
        }
        tok->bufferCurrent++;
    }

    return Tokenizer_NewIdentifier(tok, tok->currentToken, 
                                   tok->bufferCurrent - tok->currentToken);
}

struct Token *Tokenizer_RecognizeString(struct Tokenizer *tok)
{
    if (*tok->bufferCurrent != '\"') {
        tok->errorCode = TOK_ERR_ImpossibleState;
        return NULL;
    }
    tok->bufferCurrent++;

    while (tok->bufferCurrent < tok->bufferEnd) {
        if (*tok->bufferCurrent == '\"') {
            if (*(tok->bufferCurrent - 1) == '\\') {
                tok->bufferCurrent++;
                continue;
            }
            tok->bufferCurrent++;
            return Tokenizer_NewString(tok, tok->currentToken, 
                                       tok->bufferCurrent - tok->currentToken);
        }

        if (*tok->bufferCurrent == '\n') {
            tok->errorCode = TOK_ERR_AbruptEndOfString;
            return NULL;
        }

        tok->bufferCurrent++;
    }

    tok->errorCode = TOK_ERR_AbruptEndOfString;
    return NULL;
}

struct Token *Tokenizer_RecognizeOperator(struct Tokenizer *tok)
{
    char lookAheadOne = (tok->bufferCurrent + 1 < tok->bufferEnd) ? *(tok->bufferCurrent + 1) : '\0';
    char lookAheadTwo = (tok->bufferCurrent + 2 < tok->bufferEnd) ? *(tok->bufferCurrent + 2) : '\0';  

    switch (*tok->bufferCurrent) {
        case '*':  
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '*':
                    tok->bufferCurrent++;
                    switch (lookAheadTwo) {
                        case '=': tok->bufferCurrent++;
                                  return Tokenizer_NewOperator(tok, TOKEN_OP_PowEqual);
                        default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Pow);
                    }
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_MulEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Mul);
            }

        case '/':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_DivEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Div);
            }

        case '+':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '+': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Increment);
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_AddEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Add);
            }

        case '-':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '-': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Decrement);
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_SubEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Sub);
            }

        case '(':
            tok->bufferCurrent++; 
            return Tokenizer_NewOperator(tok, TOKEN_OP_Lround);

        case ')':
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Rround);

        case '[':  
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Lsquare);

        case ']':  
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Rsquare);

        case '{':  
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Lcurly);

        case '}': 
            tok->bufferCurrent++; 
            return Tokenizer_NewOperator(tok, TOKEN_OP_Rcurly);

        case '>':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '>': 
                    tok->bufferCurrent++;
                    switch (lookAheadTwo) {
                        case '=': tok->bufferCurrent++;
                                  return Tokenizer_NewOperator(tok, TOKEN_OP_BitshrEqual);
                        default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Bitshr);
                    }
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Greatereq);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Greater);
            }

        case '<':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '<': 
                    tok->bufferCurrent++;
                    switch (lookAheadTwo) {
                        case '=': tok->bufferCurrent++;
                                  return Tokenizer_NewOperator(tok, TOKEN_OP_BitshlEqual);
                        default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Bitshl);
                    }
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Lesseq);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Less);
            }

        case '=':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Equals);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Assignment);
            }

        case '.':  
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Dot);

        case ',': 
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Comma);

        case '|':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '|': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Lor);
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_BitorEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Bitor);
            }

        case '&':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '&': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Land);
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_BitandEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Bitand);
            }

        case '~':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_BitnotEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Bitnot);
            }

        case '^':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_BitxorEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Bitxor);
            }

        case '%':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_ModEqual);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Mod);
            }

        case ':':
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Colon);

        case ';':
            tok->bufferCurrent++;
            return Tokenizer_NewOperator(tok, TOKEN_OP_Semicolon);

        case '!':
            tok->bufferCurrent++;
            switch (lookAheadOne) {
                case '=': tok->bufferCurrent++;
                          return Tokenizer_NewOperator(tok, TOKEN_OP_Nequals);
                default:  return Tokenizer_NewOperator(tok, TOKEN_OP_Lnot);
            }
    }

    tok->errorCode = TOK_ERR_ImpossibleOperator;
    return NULL;
}

struct Token *Tokenizer_RecognizeNumber(struct Tokenizer *tok)
{
    int64_t number = 0; 
    while (tok->bufferCurrent < tok->bufferEnd && IsDigit(*tok->bufferCurrent)) {
        number *= 10;
        number += *tok->bufferCurrent - '0';
        tok->bufferCurrent++;
    }
    number *= 100;

    if (*tok->bufferCurrent == '.') {
        tok->bufferCurrent++;
        int multiplier = 10;

        while (tok->bufferCurrent < tok->bufferEnd && IsDigit(*tok->bufferCurrent)) {
            number += (*tok->bufferCurrent - '0') * multiplier;
            multiplier /= 10;
            tok->bufferCurrent++;
        }
    }

    return Tokenizer_NewNumber(tok, number);
}

struct Token *Tokenizer_NewIdentifier(struct Tokenizer *tok, char *identifier, int identifierLength)
{
    struct Token *token = calloc(1, sizeof (struct Token));
    if (token == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        return token;
    }

    if (IsKeyword(identifier, identifierLength)) {
        token->type = TOKEN_TYPE_Operator;
        token->operator = GetKeyword(identifier);
        return token;
    }

    token->type = TOKEN_TYPE_Identifier;
    token->identifier = calloc(identifierLength + 1, sizeof (char));
    if (token->string == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        free(token);
        return NULL;
    }
    strncpy(token->identifier, identifier, identifierLength);
    return token;
}

struct Token *Tokenizer_NewString(struct Tokenizer *tok, char *string, int stringLength)
{
    struct Token *token = calloc(1, sizeof (struct Token));
    if (token == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        return token;
    }
    token->type = TOKEN_TYPE_String;
    token->string = calloc(stringLength + 1, sizeof (char));
    if (token->string == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        free(token);
        return NULL;
    }
    strncpy(token->string, string, stringLength);
    return token;
}

struct Token *Tokenizer_NewOperator(struct Tokenizer *tok, enum Token_Code code)
{
    struct Token *token = calloc(1, sizeof (struct Token));
    if (token == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        return token;
    }
    token->type = TOKEN_TYPE_Operator;
    token->operator = code;
    return token;
}

struct Token *Tokenizer_NewNumber(struct Tokenizer *tok, int64_t number)
{
    struct Token *token = calloc(1, sizeof (struct Token));
    if (token == NULL) {
        tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
        return token;
    }
    token->type = TOKEN_TYPE_Number;
    token->number = number;
    return token;
}

struct Tokenizer *Tokenizer_Delete(struct Tokenizer *tok)
{
    if (tok == NULL) {
        return tok;
    }
    tok->lineNumber = 0;
    tok->errorCode  = TOK_ERR_NoError;
    
    fclose(tok->file);
    tok->file = NULL;

    free(tok->bufferStart);
    tok->bufferStart   = NULL;
    tok->bufferCurrent = NULL;
    tok->bufferEnd     = NULL;
    tok->currentToken  = NULL;
    tok->currentLine   = NULL;
    
    free(tok);
    return NULL;
}
