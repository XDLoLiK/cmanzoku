#include "parser.h"

struct Parser *Parser_New(const char *fileName)
{
	struct Parser *par = calloc(1, sizeof (struct Parser));
	if (par == NULL) {
		return par;
	}
	par->fileName = fileName;
	
	par->tok = Tokenizer_New(fileName);
	if (par->tok->state = TOK_STATE_TokenizerError) {
		par->errorCode = PARSER_ERR_TokenizerError;
		return par;
	}

	par->lineNumber = 1;
	par->errorCode = PARSER_ERR_NoError;
	return par;
}

struct Tree_Node *Parser_GetSyntaxTree(struct Parser *par)
{
	GetGrammar(par);
}

static struct Tree_Node *Parser_GetGrammar(struct Parser *par)
{
	par->treeRoot = getNumber(par);
}

static struct Tree_Node *Parser_GetNumber(struct Parser *par)
{

}

static struct Tree_Node *Parser_GetUnarySign(struct Parser *par)
{

}

static struct Tree_Node *Parser_GetIdentifier(struct Parser *par)
{
	
}

struct Parser *Parser_Delete(struct Parser *par)
{
	par->errorCode = PARSER_ERR_NoError;
	par->lineNumber = 0;
	par->fileName = NULL;
	par->treeRoot = Tree_Delete(par->treeRoot);
	par->tok = Tokenizer_Delete(par->tok);
	free(par);
	return NULL;
}
