#ifndef Man_PARSER_H
#define Man_PARSER_H

#include "Manzoku.h"
#include "tokenizer.h"

#define RAISE(error) {				\
	parser->errorCode = error;		\
	Man_PrintParserError(parser);   \
}

#define ADVANCE(par) {										\
	par->currentToken = Tokenizer_GetNextToken(par->tok);	\
	if (par->currentToken == NULL) {						\
		RAISE(PARSER_ERR_TokenizerError);					\
	}														\
}

#define REQUIRE_TYPE(t) {						\
	if (parser->currentToken->type != t) {		\
		RAISE(PARSER_ERR_WrongTokenType);		\
	}											\
}

#define REQUIRE_OPERATOR(op) {					\
	if (parser->currentToken->operator != op) {	\
		RAISE(PARSER_ERR_WrongOperator);		\
	}											\
}

enum Parser_Error {
	PARSER_ERR_NoError                = 0,
	PARSER_ERR_UnsuccessfulAllocation = 1,
	PARSER_ERR_TokenizerError         = 2,
	PARSER_ERR_WrongTokenType         = 3,
	PARSER_ERR_WrongOperator          = 4,
	PARSER_ERR_ImpossibleOperator     = 5
};

struct Parser {
	struct Tokenizer *tok;
	const char *fileName;
	enum Parser_Error errorCode;
	struct Token *currentToken;
};

// Main parse function
struct Tree_Node *Parser_GetGrammar(struct Parser *parser);

// Create/delete
struct Parser *Parser_New(const char *fileName);
struct Parser *Parser_Delete(struct Parser *parser);

// Recursive descent
struct Tree_Node *Parser_GetMultilineOperator(struct Parser *parser);
struct Tree_Node *Parser_GetVarDecl(struct Parser *parser);
struct Tree_Node *Parser_GetIf(struct Parser *parser);
struct Tree_Node *Parser_GetFor(struct Parser *parser);
struct Tree_Node *Parser_GetWhile(struct Parser *parser);
struct Tree_Node *Parser_GetFunction(struct Parser *parser);
struct Tree_Node *Parser_GetParametersList(struct Parser *parser);
struct Tree_Node *Parser_GetGetAddress(struct Parser *parser);
struct Tree_Node *Parser_GetAtAddress(struct Parser *parser);	
struct Tree_Node *Parser_GetExpression(struct Parser *parser);
struct Tree_Node *Parser_GetComma(struct Parser *parser);
struct Tree_Node *Parser_GetAssignment(struct Parser *parser);
struct Tree_Node *Parser_GetLogicalOr(struct Parser *parser);
struct Tree_Node *Parser_GetLogicalAnd(struct Parser *parser);
struct Tree_Node *Parser_GetBitOr(struct Parser *parser);
struct Tree_Node *Parser_GetBitXor(struct Parser *parser);
struct Tree_Node *Parser_GetBitAnd(struct Parser *parser);
struct Tree_Node *Parser_GetEquality(struct Parser *parser);
struct Tree_Node *Parser_GetComparison(struct Parser *parser);
struct Tree_Node *Parser_GetBitShift(struct Parser *parser);
struct Tree_Node *Parser_GetAddSub(struct Parser *parser);
struct Tree_Node *Parser_GetMulDivMod(struct Parser *parser);
struct Tree_Node *Parser_GetUnarySign(struct Parser *parser);
struct Tree_Node *Parser_GetParenthesis(struct Parser *parser);
struct Tree_Node *Parser_GetString(struct Parser *parser);
struct Tree_Node *Parser_GetNumber(struct Parser *parser);
struct Tree_Node *Parser_GetIdentifier(struct Parser *parser);

// External
void Man_PrintParserError(struct Parser *parser);

#endif // Man_PARSER_H
