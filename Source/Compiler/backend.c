#include "backend.h"

extern int DUMP_AST_FLAG; 
extern int CREATE_LISTING_FLAG;

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

	if (DUMP_AST_FLAG) {
		fclose(treeFile);
	}
	else {
		remove((const char *)treeFile);
	}

	struct Compiler *compiler = Compiler_New(fileName);
	if (compiler == NULL) {
		return 1;
	}

	if (CREATE_LISTING_FLAG) {
		Compiler_CreateListing(compiler, syntaxTree);
	}

	/* NOT IMPLEMENTED YET */
	// Compiler_CreateExecutable(compiler, syntaxTree);

	syntaxTree = Tree_Delete(syntaxTree);
	compiler = Compiler_Delete(compiler);
	return 0;
}