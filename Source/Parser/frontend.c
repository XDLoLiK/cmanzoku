#include "parser.h"

int Frontend_Main(const char *fileName)
{
	struct Parser *parser = Parser_New(fileName);
	if (parser == NULL) {
		return 1;
	}

	struct Tree_Node* syntaxTreeRoot = Parser_GetGrammar(parser);
	if (syntaxTreeRoot == NULL) {
		return 1;
	}

	char *treeFileName = calloc(strlen(fileName) + 10, sizeof (char));
	if (treeFileName == NULL) {
		return 1;
	}
	strcpy(treeFileName, fileName);
	strcat(treeFileName, ".tree.txt");
	
	FILE *treeFile = fopen(treeFileName, "w");
	if (treeFile == NULL) {
		free(treeFileName);
		return 1;
	}

#ifdef OPTIMIZATION_ON
	Tree_Optimize(&syntaxTreeRoot);
#endif // OPTIMIZATION_ON

	Tree_Upload(syntaxTreeRoot, treeFile, 0);

#ifdef DEBUG_ON
	Tree_CreateGraph(syntaxTreeRoot, fileName);
#endif // DEBUG_ON

	free(treeFileName);	
	fclose(treeFile);
	parser = Parser_Delete(parser);
	syntaxTreeRoot = Tree_Delete(syntaxTreeRoot);
	return 0;
}

int Frontend_DumpTokens(const char *fileName)
{
	struct Tokenizer *tok = Tokenizer_New(fileName);
	if (tok == NULL) {
		return 1;
	}
	struct Token *currentToken = Tokenizer_GetNextToken(tok);

	char *tokenFileName = calloc(strlen(fileName) + 8, sizeof (char));
	if (tokenFileName == NULL) {
		return 1;
	}
	strcpy(tokenFileName, fileName);
	strcat(tokenFileName, ".tokens");

	FILE *tokenFile = fopen(tokenFileName, "w");
	if (tokenFile == NULL) {
		return 1;
	}

	for ( ; currentToken->type != TOKEN_TYPE_Endmarker; ) {
		switch (currentToken->type) {

			case TOKEN_TYPE_Number:
				fprintf(tokenFile, "NUMBER:    %" PRId64 "\n", currentToken->number);
				break;

			case TOKEN_TYPE_String:
				fprintf(tokenFile, "STRING:    %s\n", currentToken->string);
				break;

			case TOKEN_TYPE_Identifier:
				fprintf(tokenFile, "IDENTIFER: %s\n", currentToken->identifier);
				break;

			case TOKEN_TYPE_Operator:
				fprintf(tokenFile, "OPERATOR:  %d\n", currentToken->operator);
				break;

			default:
				fprintf(tokenFile, "ERROR");
				break;
		}

		currentToken = Tokenizer_GetNextToken(tok);
		if (currentToken == NULL) {
			Man_PrintError(currentToken, "");
		}
	}
	fprintf(tokenFile, "END\n");

	tok = Tokenizer_Delete(tok);
	return 0;
}
