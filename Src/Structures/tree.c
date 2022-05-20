#include "Structures/tree.h"

struct Tree_Node *Tree_NewNode(struct Token *token)
{
	struct Tree_Node *newNode = calloc(1, sizeof (struct Tree_Node));
	if (newNode == NULL || token == NULL) {
		free(newNode);
		return NULL;
	}
	newNode->token = token;
	return newNode;
}

void Tree_Upload(struct Tree_Node *root, FILE *file, int depth)
{
	PRINT_N_TABS(depth);
	fprintf(file, "{\n");
	
	if (root == NULL) {
		PRINT_N_TABS(depth + 1);
		fprintf(file, "nil\n");
	}
	else {
		PRINT_N_TABS(depth + 1);
		fprintf(file, "new\n");

		PRINT_N_TABS(depth + 1);
		fprintf(file, "type: %d\n", root->token->type);

		PRINT_N_TABS(depth + 1);
		switch (root->token->type) {
			case TOKEN_TYPE_Endmarker:
				fprintf(file, "value: %d\n", root->token->operator);
				break;

			case TOKEN_TYPE_Number:
				fprintf(file, "value: %" PRId64 "\n", root->token->number);
				break;
				
			case TOKEN_TYPE_String:
				fprintf(file, "value: %s\n", root->token->string);
				break;
			
			case TOKEN_TYPE_Identifier:
				fprintf(file, "value: %s\n", root->token->identifier);
				break;
			
			case TOKEN_TYPE_Operator:
				fprintf(file, "value: %d\n", root->token->operator);
				break;
		}
		
		Tree_Upload(root->left,  file, depth + 1);
		Tree_Upload(root->right, file, depth + 1);
	}

	PRINT_N_TABS(depth);
	fprintf(file, "}\n");
	return;
}

struct Tree_Node *Tree_Download(FILE *file)
{
	char formatCheck  [2] = "";
	char contentsCheck[4] = "";

	fscanf(file, "%1s", formatCheck);
	if (strcmp(formatCheck, "{") != 0) {
		fprintf(stderr, "\033[0;31m" "ERROR:" "\033[0m" "Invalid AST format\n");
		exit(0);
	}

	fscanf(file, "%8s", contentsCheck);
	if (strcmp(contentsCheck, "nil") == 0) {
		fscanf(file, "%1s", formatCheck);
		if (strcmp(formatCheck, "}") != 0) {
			fprintf(stderr, "\033[0;31m" "ERROR:" "\033[0m" "Invalid AST format\n");
			exit(0);
		}
		return NULL;
	}

	struct Token *token = calloc(1, sizeof (struct Token));
	if (token == NULL) {
		return NULL;
	}
	fscanf(file, " type: %u", &token->type);
	
	switch (token->type) {
		case TOKEN_TYPE_Endmarker:
			fscanf(file, " value: %u", &token->operator);
			break;

		case TOKEN_TYPE_Number:
			fscanf(file, " value: %" PRId64, &token->number);
			break;
			
		case TOKEN_TYPE_String:
			token->string = calloc(64, sizeof (char));
			fscanf(file, " value: %64[^\n]", token->string);
			break;
		
		case TOKEN_TYPE_Identifier:
			token->identifier = calloc(64, sizeof (char));
			fscanf(file, " value: %64s", token->identifier);
			break;
		
		case TOKEN_TYPE_Operator:
			fscanf(file, " value: %u", &token->operator);
			break;
	}

	struct Tree_Node *root = Tree_NewNode(token);
	root->left  = Tree_Download(file);
	root->right = Tree_Download(file);

	fscanf(file, "%1s", formatCheck);
	if (strcmp(formatCheck, "}") != 0) {
		fprintf(stderr, "\033[0;31m" "ERROR:" "\033[0m" "Invalid AST format\n");
		exit(0);
	}
	return root;
}

struct Tree_Node *Tree_Delete(struct Tree_Node *root)
{
	if (root == NULL) {
		return root;
	}
	root->left  = Tree_Delete(root->left);
	root->right = Tree_Delete(root->right);
	free(root->token);
	free(root);
	return NULL;
}
