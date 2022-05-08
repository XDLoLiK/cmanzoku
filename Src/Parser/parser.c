#include "parser.h"

struct Parser *Parser_New(const char *fileName)
{
	struct Parser *parser = calloc(1, sizeof (struct Parser));
	if (parser == NULL) {
		return parser;
	}
	parser->fileName = fileName;
	
	parser->tok = Tokenizer_New(fileName);
	if (parser->tok->errorCode != TOK_ERR_NoError) {
		parser->errorCode = PARSER_ERR_TokenizerError;
		return parser;
	}

	parser->lineNumber = 1;
	parser->errorCode = PARSER_ERR_NoError;
	return parser;
}

// struct Tree_Node *Parser_GetSyntaxTree(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetGrammar(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetAssignment(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetFunction(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetWhile(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetFor(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetIfElse(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetLogical(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetAddSub(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetMulDiv(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetPower(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetBitOpeartion(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetParenthesisExpression(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetNumber(struct Parser *parser)
// {
// 	struct Token *token = Tokenizer_GetNextToken(parser->tok);
// 	// REQUIRE_TYPE(TOKEN_TYPE_Number, token);
// 	return token;
// }

// static struct Tree_Node *Parser_GetKeyword(struct Parser *parser)
// {

// }

// static struct Tree_Node *Parser_GetUnarySign(struct Parser *parser)
// {
	
// }

// static struct Tree_Node *Parser_GetIdentifier(struct Parser *parser)
// {
	
// }

struct Parser *Parser_Delete(struct Parser *parser)
{
	parser->errorCode = PARSER_ERR_NoError;
	parser->lineNumber = 0;
	parser->fileName = NULL;
	parser->treeRoot = Tree_Delete(parser->treeRoot);
	parser->tok = Tokenizer_Delete(parser->tok);
	free(parser);
	return NULL;
}
