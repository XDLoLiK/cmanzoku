#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Manzoku.h"

#define CUR_SYMBOL (*tok->bufferCurrent)

enum Tokenizer_Error {
    TOK_ERR_NoError                 = 0,
    TOK_ERR_UnsuccessfulAllocation  = 1,
    TOK_ERR_AbruptEndOfString       = 2,
    TOK_ERR_AbruptEndOfComment      = 3,
    TOK_ERR_ImpossibleOperator      = 4,
    TOK_ERR_ImpossibleState         = 5,
    TOK_ERR_NotExistingFile         = 6,
    TOK_ERR_ImpossibleTokenType     = 7,
    TOK_ERR_ImpossibleCharSequence  = 8,
    TOK_ERR_StringLengthExceeded    = 9,
    TOK_ERR_NumberPrecisionExceeded = 10
};

struct Tokenizer {
    char *currentLine;
    char *currentToken;
    char *bufferStart;
    char *bufferCurrent;
    char *bufferEnd;
    char *fileName;
    FILE *file;
    enum Tokenizer_Error errorCode;
    int lineNumber;
};

// Main tokenizing function
struct Token *Tokenizer_GetNextToken(struct Tokenizer *tok);

// Create/delete
struct Tokenizer *Tokenizer_New(const char *fileName);
struct Tokenizer *Tokenizer_Delete(struct Tokenizer *tok);

// Auxiliary functions
void Tokenizer_RecognizeStartOfData(struct Tokenizer *tok);
void Tokenizer_RecognizeCommentary(struct Tokenizer *tok);
struct Token *Tokenizer_RecognizeEndOfData(struct Tokenizer *tok);
struct Token *Tokenizer_RecognizeIdentifier(struct Tokenizer *tok);
struct Token *Tokenizer_RecognizeString(struct Tokenizer *tok);
struct Token *Tokenizer_RecognizeOperator(struct Tokenizer *tok);
struct Token *Tokenizer_RecognizeNumber(struct Tokenizer *tok);

// Token creators
struct Token *Tokenizer_NewIdentifier(struct Tokenizer *tok, char *identifier, int identifierLength);
struct Token *Tokenizer_NewString(struct Tokenizer *tok, char *string, int stringLength);
struct Token *Tokenizer_NewOperator(struct Tokenizer *tok, enum Token_Code code);
struct Token *Tokenizer_NewNumber(struct Tokenizer *tok, int64_t number);

#endif // TOKENIZER_H
