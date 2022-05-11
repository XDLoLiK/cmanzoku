#include "parser.h"

int Frontend_Main(const char *fileName)
{
	struct Parser *parser = Parser_New(fileName);
	struct Tree_Node* syntaxTreeRoot = Parser_GetSyntaxTree(parser);

	char *treeFileName = calloc(strlen(fileName) + 6, sizeof (char));
	strcpy(treeFileName, fileName);
	strcat(treeFileName, ".tree");
	
	FILE *treeFile = fopen(treeFileName, "w");
	Tree_Upload(syntaxTreeRoot, treeFile, 0);

	free(treeFileName);	
	fclose(treeFile);
	parser = Parser_Delete(parser);
	syntaxTreeRoot = Tree_Delete(syntaxTreeRoot);
	return 0;
}

void Frontend_DumpTokens(const char *fileName)
{
	struct Tokenizer *tok = Tokenizer_New(fileName);
	struct Token *currentToken = Tokenizer_GetNextToken(tok);

	for ( ; currentToken->type != TOKEN_TYPE_Endmarker; ) {
		switch (currentToken->type) {
			case TOKEN_TYPE_Endmarker:
				printf("END\n");
				break;
			
			case TOKEN_TYPE_Number:
				printf("NUMBER:    %" PRId64 "\n", currentToken->number);
				break;

			case TOKEN_TYPE_String:
				printf("STRING:    %s\n", currentToken->string);
				break;

			case TOKEN_TYPE_Identifier:
				printf("IDENTIFER: %s\n", currentToken->identifier);
				break;

			case TOKEN_TYPE_Operator:
				printf("OPERATOR:  %d\n", currentToken->operator);
				break;

			default:
				printf("ERROR");
				break;
		}

		currentToken = Tokenizer_GetNextToken(tok);
		if (currentToken == NULL) {
			Man_PrintTokError(tok);
		}
	}
	printf("END\n");

	tok = Tokenizer_Delete(tok);
	return;
}
