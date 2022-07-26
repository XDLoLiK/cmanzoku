#include "compiler.h"

int Backend_Main(const char *fileName)
{
	char *treeFileName = calloc(strlen(fileName) + 10, sizeof (char));
	if (treeFileName == NULL) {
		return 1;
	}
	strcpy(treeFileName, fileName);
	strcat(treeFileName, ".tree.txt");

	FILE *treeFile = fopen(treeFileName, "r");
	if (treeFile == NULL) {
		return 1;
	}

	struct Tree_Node *syntaxTree = Tree_Download(treeFile);
	if (syntaxTree == NULL) {
		return 1;
	}
	free(treeFileName);	
	fclose(treeFile);

	struct Compiler *compiler = Compiler_New(fileName);
	if (compiler == NULL) {
		return 1;
	}
	Compiler_CreateListing(compiler, syntaxTree);

	/* NOT IMPLEMENTED YET */
	// Compiler_CreateExecutable(compiler, syntaxTree);

	syntaxTree = Tree_Delete(syntaxTree);
	compiler = Compiler_Delete(compiler);
	return 0;
}