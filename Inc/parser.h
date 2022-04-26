#ifndef Man_PARSER_H
#define Man_PARSER_H

#include "Manzoku.h"
#include "tokenizer.h"

enum Parser_Error {
	PARSER_ERR_NoError                = 0,
	PARSER_ERR_UnsuccessfulAllocation = 1,
	PARSER_ERR_NotExistingFile        = 2,
	PARSER_ERR_TokenizerError         = 3
};

struct Parser {
	struct Tokenizer *tok;
	const char *fileName;
	enum Parser_Error errorCode;
	int lineNumber;
};

// Main parse function
struct Tree_Node *Parser_GetSyntaxTree(struct Parser *par);

// Create/delete
struct Parser *Parser_New(const char *fileName);
struct Parser *Parser_Delete(struct Parser *par);

// Recursive descent
// ...

#endif // Man_PARSER_H
