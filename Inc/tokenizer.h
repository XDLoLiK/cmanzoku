#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "Manzoku.h"

enum Tokenizer_State {
	TOK_STATE_TokenizerError  = 0,
	TOK_STATE_EndOfData       = 1,
	TOK_STATE_FindStartOfData = 2,
	TOK_STATE_Identifier      = 3,
	TOK_STATE_String          = 4,
	TOK_STATE_Number          = 5,
	TOK_STATE_Operator        = 6,
	TOK_STATE_Commentary      = 7
};

enum Tokenizer_Error {
	TOK_ERR_NoError                = 0,
	TOK_ERR_UnsuccessfulAllocation = 1,
	TOK_ERR_AbruptEndOfString      = 2,
	TOK_ERR_AbruptEndOfComment     = 3,
	TOK_ERR_ImpossibleOperator     = 4,
	TOK_ERR_ImpossibleState		   = 5,
	TOK_ERR_NotExistingFile        = 6
};

struct Tokenizer {
	char *currentToken;
	char *bufferStart;
	char *bufferCurrent;
	char *bufferEnd;
	FILE *file;
	enum Tokenizer_State state;
	enum Tokenizer_Error errorCode;
	int lineNumber;
};

// Main tokenizing function
struct Token *Tokenizer_GetNextToken(struct Tokenizer *tok);

// Create/delete
struct Tokenizer *Tokenizer_New(const char *fileName);
struct Tokenizer *Tokenizer_Delete(struct Tokenizer *tok);

// Auxiliary functions
static struct Tokenizer *Tokenizer_CreateNewToken(struct Tokenizer *tok)

#endif // TOKENIZER_H
