#include "compiler.h"

struct Compiler *Compiler_New(const char *sourceName)
{
    struct Compiler *compiler = calloc(1, sizeof (struct Compiler));
    if (compiler == NULL) {
        return compiler;
    }

    compiler->executableName = calloc(strlen(sourceName) + 5, sizeof (char));
    compiler->listingName    = calloc(strlen(sourceName) + 5, sizeof (char));
    if (compiler->executableName == NULL || compiler->listingName == NULL) {
        compiler = Compiler_Delete(compiler);
        return NULL;
    }
    strcpy(compiler->currentScope,   "global");
    strcpy(compiler->executableName, sourceName);
    strcpy(compiler->listingName,    sourceName);
    strcat(compiler->executableName, ".out");
    strcat(compiler->listingName,    ".asm");

    compiler->executableFile = fopen(compiler->executableName, "w");
    compiler->listingFile    = fopen(compiler->listingName,    "w");
    if (compiler->executableFile == NULL || compiler->listingFile == NULL) {
        compiler = Compiler_Delete(compiler);
        return NULL;
    }

    compiler->identifiersList = HashTable_New((1 << 16) + 1, MAX_IDENTIFIER_LENGTH, 32);
    if (compiler->identifiersList == NULL) {
        compiler = Compiler_Delete(compiler);
        return NULL;
    }
    return compiler;
}

int Compiler_CountLocalVariables(struct Compiler *compiler, struct Tree_Node *functionNode)
{
    if (functionNode == NULL) {
        return 0;
    }

    int count = 0;
    if (functionNode->token->type == TOKEN_TYPE_Operator && 
        functionNode->token->operator == TOKEN_KW_VarDecl) {
        count++;
    }
    count += Compiler_CountLocalVariables(compiler, functionNode->left);
    count += Compiler_CountLocalVariables(compiler, functionNode->right);
    return count;
}

struct Compiler *Compiler_Delete(struct Compiler *compiler)
{
    compiler->identifiersList = HashTable_Delete(compiler->identifiersList);
    if (compiler->executableFile == NULL) {
        fclose(compiler->executableFile);
    }
    if (compiler->listingFile == NULL) {
        fclose(compiler->listingFile);
    }
    free(compiler->listingName);    
    free(compiler->executableName);
    free(compiler);
    return NULL;
}
