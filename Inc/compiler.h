#ifndef Man_COMPILER_H
#define Man_COMPILER_H

#include "Manzoku.h"

enum CompilerErrors {
	COMPILER_ERR_NoError            = 0,
	COMPILER_ERR_ImpossibleOperator = 1
};

struct Compiler {
	enum CompilerErrors errorCode;
	char *executableName;
	char *listingName;
	FILE *executableFile;
	FILE *listingFile;
	struct Tree_Node *syntaxTree;
	int labelCount;
	struct HashTable *identifiersList;
	char currentScope[MAX_IDENTIFIER_LENGTH];
	int localVarCount;
	int localVarAmount;
};

// Create/Delete
struct Compiler *Compiler_New(const char *sourceName);
struct Compiler *Compiler_Delete(struct Compiler *compiler);

// Compiling
int Compiler_CountLocalVariables(struct Compiler *compiler, struct Tree_Node *functionNode);

// Listing
int Compiler_CreateListing(struct Compiler *compiler, struct Tree_Node *syntaxTree);
int Compiler_CreateListingPreamble(struct Compiler *compiler);
int Compiler_GrammarListing(struct Compiler *compiler, struct Tree_Node *grammarNode);
int Compiler_VarDeclListing(struct Compiler *compiler, struct Tree_Node *varDecLNode);
int Compiler_FunctionArgsListing(struct Compiler *compiler, struct Tree_Node *functionNode);
int Compiler_MultilineOperatorListing(struct Compiler *compiler, struct Tree_Node *operatorNode);
int Compiler_IfStatementListing(struct Compiler *compiler, struct Tree_Node *ifNode);
int Compiler_FunctionParamsListing(struct Compiler *compiler, struct Tree_Node *functionNode);
int Compiler_WhileStatementListing(struct Compiler *compiler, struct Tree_Node *whileNode);
int Compiler_ExpressionListing(struct Compiler *compiler, struct Tree_Node *expressionNode);
int Compiler_CountArgumetsAmount(struct Compiler *compiler, struct Tree_Node *argsNode);
int Compiler_FunctionListing(struct Compiler *compiler, struct Tree_Node *functionNode);
int Compiler_FunctionPreambleListing(struct Compiler *compiler);
int Compiler_FunctionPostambleListing(struct Compiler *compiler);
int Compiler_CreateListingPostamble(struct Compiler *compiler);

// External
void Man_PrintCompilerError(struct Compiler *compiler);

/* NOT IMPLEMENTED YET */
int Compiler_ForStatementListing(struct Compiler *compiler, struct Tree_Node *forNode);

#endif // Man_COMPILER_H
