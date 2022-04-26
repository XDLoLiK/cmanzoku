#include "tokenizer.h"

#define CUR_SYMBOL (*(tok->bufferCurrent))

#define IS_DIGIT(symbol) ((symbol >= '0') && (symbol <= '9'))

#define IS_VALID_IDENTIFIER_START(symbol)  ((symbol == '_') || \
											(symbol >= 'a'  &&  symbol <= 'z') || \
											(symbol >= 'A'  &&  symbol <= 'Z'))

#define IS_VALID_IDENTIFIER_SYMBOL(symbol) ((symbol == '_') || \
 									 	   	(symbol >= 'a'  &&  symbol <= 'z') || \
 									 	   	(symbol >= 'A'  &&  symbol <= 'Z') || \
 									 		(symbol >= '0'  &&  symbol <= '9'))

#define IS_INESSENTIAL_SPACE(symbol)	   ((symbol == '\t') || \
											(symbol == ' ')  || (symbol == '\r') || \
											(symbol == '\f') || (symbol == '\v'))

struct Tokenizer *Tokenizer_New(const char *fileName)
{
	struct Tokenizer *tok = calloc(1, sizeof (struct Tokenizer));
	if (tok == NULL) {
		return tok;
	}
	tok->file = fopen(fileName, "r");
	if (tok->file == NULL) {
		tok->state = TOK_STATE_TokenizerError;
		tok->errorCode = TOK_ERR_NotExistingFile;
		return tok;
	}

	tok->bufferStart = calloc(GetFileSize(tok->file), sizeof (char));
	if (tok->bufferStart == NULL) {
		tok->state = TOK_STATE_TokenizerError;
		tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
		return tok;
	}
	fread(tok->bufferStart, sizeof (char), GetFileSize(tok->file), tok->file);
	tok->bufferCurrent = tok->bufferStart;
	tok->bufferEnd = tok->bufferStart + sizeof tok->bufferStart;	
	tok->currentToken = tok->bufferStart;
	
	tok->tokenList = calloc(1, sizeof (struct Token *));
	if (tok->tokenList == NULL) {
		tok->state = TOK_STATE_TokenizerError;
		tok->errorCode = TOK_ERR_UnsuccessfulAllocation;
		return tok;
	}
	
	tok->state = TOK_STATE_FindStartOfData;
	tok->errorCode = TOK_ERR_NoError;
	tok->lineNumber = 1;
	return tok;
}

struct Token *Tokenizer_GetNextToken(struct Tokenizer *tok)
{
	for (bool running = true; running; tok->bufferCurrent++) {
		switch (tok->state) {

			case TOK_STATE_TokenizerError:
				Man_PrintTokError(tok);
				running = false;
				break;

			case TOK_STATE_EndOfData:
				// TODO
				running = false;
				break;
			
			case TOK_STATE_FindStartOfData:
				if (tok->bufferCurrent == tok->bufferEnd) {
					tok->state = TOK_STATE_EndOfData;
					break;
				}
				else if (IS_INESSENTIAL_SPACE(CUR_SYMBOL)) {
					continue;
				}
				else if (IS_VALID_IDENTIFIER_START(CUR_SYMBOL)) {
					tok->currentToken = tok->bufferCurrent;
					tok->state = TOK_STATE_Identifier;
					break;
				}
				else if (IS_DIGIT(CUR_SYMBOL)) {
					tok->currentToken = tok->bufferCurrent;
					tok->state = TOK_STATE_Number;
					break;
				}
				else if (CUR_SYMBOL == '"') {
					tok->currentToken = tok->bufferCurrent;
					tok->state = TOK_STATE_String;
					break;
				}
				else if (CUR_SYMBOL == '$') {
					tok->currentToken = tok->bufferCurrent;
					tok->state = TOK_STATE_Commentary;
					break;
				}
				else {
					tok->currentToken = tok->bufferCurrent;
					tok->state = TOK_STATE_Operator;
					break;
				}
			
			case TOK_STATE_Identifier:
				if (tok->bufferCurrent == tok->bufferEnd) {
					tok->state = TOK_STATE_EndOfData;
					// TODO
					break;
				}
				else if (IS_VALID_IDENTIFIER_SYMBOL(CUR_SYMBOL)) {
					continue;
				}
				else if (IS_INESSENTIAL_SPACE(CUR_SYMBOL)) {
					// TODO
				}
				else {
					break;
				}
			
			case TOK_STATE_String:
				if (tok->bufferCurrent == tok->bufferEnd) {
					tok->state = TOK_STATE_TokenizerError;
					tok->errorCode = TOK_ERR_AbruptEndOfString;
					break;
				}
				else if (CUR_SYMBOL == '"') {
					tok->state = TOK_STATE_FindStartOfData;
					// TODO
					break;
				}
				else {
					continue;
				}
			
			case TOK_STATE_Number:
				if (tok->bufferCurrent == tok->bufferEnd) {
					tok->state = TOK_STATE_EndOfData;
					// TODO
					break;
				}
				else if (IS_DIGIT(CUR_SYMBOL)) {
					continue;
				}
				else () {
					// TODO
				}
				break;
			
			case TOK_STATE_Operator:
				if (tok->bufferCurrent == tok->bufferEnd) {
					tok->state = TOK_STATE_EndOfData;
					// TODO
					break;
				}
				else if (CUR_SYMBOL == '{') {
					// TODO
				}
				else if (CUR_SYMBOL == '[') {
					
					// TODO
				}
				else if (CUR_SYMBOL == '(') {
					// TODO
				}
				else {

				}
				break;
			
			case TOK_STATE_Commentary:
				if (tok->bufferCurrent == tok->bufferEnd) {
					tok->state = TOK_STATE_TokenizerError;
					tok->errorCode = TOK_ERR_AbruptEndOfComment;
					break;
				}
				else if (CUR_SYMBOL == '$') {
					tok->state = TOK_STATE_FindStartOfData;
					break;
				}
				else {
					continue;
				}

			default:
				tok->state = TOK_STATE_TokenizerError;
				tok->errorCode = TOK_ERR_ImpossibleState;
				break;
		}
	}
	return Tokenizer_CreateNewToken(tok);
}

static struct Tokenizer *Tokenizer_CreateNewToken(struct Tokenizer *tok)
{

}

struct Tokenizer *Tokenizer_Delete(struct Tokenizer *tok)
{
	if (tok == NULL) {
		return tok;
	}
	tok->lineNumber = 0;
	tok->state = TOK_STATE_EndOfData;
	tok->errorCode = TOK_ERR_NoError;
	
	fclose(tok->file);
	tok->file = NULL;

	free(tok->bufferStart);
	tok->bufferStart = NULL;
	tok->bufferCurrent = NULL;
	tok->bufferEnd = NULL;
	tok->currentToken = NULL;
	
	free(tok->tokenList);
	tok->tokenCount = 0;
	tok->tokenCapacity = 0;
	tok->tokenList = NULL;
	
	free(tok);
	return NULL;
}
