#include "astopt.h"

int Tree_Optimize(struct Tree_Node **rootPointer)
{
	if (rootPointer == NULL) {
		return 1;
	}

	struct Tree_Node *root = *rootPointer;

	if (root->left) {
		Tree_Optimize(&root->left);	
	}
	if (root->right) {
		Tree_Optimize(&root->right);
	}
	__Tree_Optimize(rootPointer);

	return 0;
}

int __Tree_Optimize(struct Tree_Node **rootPointer)
{
	if (rootPointer == NULL) {
		return 1;
	}

	Tree_OptimizeNullAddition(rootPointer);
	Tree_OptimizeNullSubstraction(rootPointer);
	
	Tree_OptimizeNullMultiplication(rootPointer);
	Tree_OptimizeNullDivision(rootPointer);
	
	Tree_OptimizeOneMultiplication(rootPointer);
	Tree_OptimizeOneDivision(rootPointer);

	return 0;
}

int Tree_OptimizeNullAddition(struct Tree_Node **rootPointer)
{
	struct Tree_Node *root = *rootPointer;

	if (root == NULL) {
		return 1;
	}

	if (root->token->operator != TOKEN_OP_Add ||
		root->token->type     != TOKEN_TYPE_Operator) {
		return 0;
	}

	if (root->left->token->type == TOKEN_TYPE_Number && root->left->token->number == 0) {
		*rootPointer = root->right;
		root->right = NULL;
		Tree_Delete(root);
		return 0;
	}

	if (root->right->token->type == TOKEN_TYPE_Number && root->right->token->number == 0) {
		*rootPointer = root->left;
		root->left = NULL;
		Tree_Delete(root);
	}

	return 0;
}

int Tree_OptimizeNullSubstraction(struct Tree_Node **rootPointer)
{
	struct Tree_Node *root = *rootPointer;

	if (root == NULL) {
		return 1;
	}

	if (root->token->operator != TOKEN_OP_Sub ||
		root->token->type     != TOKEN_TYPE_Operator) {
		return 0;
	}

	if (root->right->token->type == TOKEN_TYPE_Number && root->right->token->number == 0) {
		*rootPointer = root->left;
		root->left = NULL;
		Tree_Delete(root);
		return 0;
	}

	if (root->left->token->type == TOKEN_TYPE_Number && root->left->token->number == 0) {
		Tree_Delete(root->left);
		root->left = NULL;
	}

	return 0;
}

int Tree_OptimizeNullMultiplication(struct Tree_Node **rootPointer)
{
	struct Tree_Node *root = *rootPointer;

	if (root == NULL) {
		return 1;
	}

	if (root->token->operator != TOKEN_OP_Mul ||
		root->token->type     != TOKEN_TYPE_Operator) {
		return 0;
	}

	if (root->left->token->type == TOKEN_TYPE_Number && root->left->token->number == 0) {
		*rootPointer = root->right;
		root->right = NULL;
		Tree_Delete(root);
		return 0;
	}

	if (root->right->token->type == TOKEN_TYPE_Number && root->right->token->number == 0) {
		*rootPointer = root->left;
		root->left = NULL;
		Tree_Delete(root);
	}

	return 0;
}

int Tree_OptimizeNullDivision(struct Tree_Node **rootPointer)
{
	struct Tree_Node *root = *rootPointer;

	if (root == NULL) {
		return 1;
	}

	if (root->token->operator != TOKEN_OP_Div ||
		root->token->type     != TOKEN_TYPE_Operator) {
		return 0;
	}

	if (root->left->token->type == TOKEN_TYPE_Number && root->left->token->number == 0) {
		*rootPointer = root->left;
		root->left = NULL;
		Tree_Delete(root);
		return 0;
	}

	if (root->right->token->type == TOKEN_TYPE_Number && root->right->token->number == 0) {
		Man_PrintError(root->right->token, "division by zero");
		return 1;
	}

	return 0;
}

int Tree_OptimizeOneMultiplication(struct Tree_Node **rootPointer)
{
	struct Tree_Node *root = *rootPointer;

	if (root == NULL) {
		return 1;
	}

	if (root->token->operator != TOKEN_OP_Mul ||
		root->token->type     != TOKEN_TYPE_Operator) {
		return 0;
	}

	if (root->left->token->type == TOKEN_TYPE_Number && root->left->token->number == 100) {
		*rootPointer = root->right;
		root->right = NULL;
		Tree_Delete(root);
		return 0;
	}

	if (root->right->token->type == TOKEN_TYPE_Number && root->right->token->number == 100) {
		*rootPointer = root->left;
		root->left = NULL;
		Tree_Delete(root);
	}

	return 0;
}

int Tree_OptimizeOneDivision(struct Tree_Node **rootPointer)
{
	struct Tree_Node *root = *rootPointer;

	if (root == NULL) {
		return 1;
	}

	if (root->token->operator != TOKEN_OP_Div ||
		root->token->type     != TOKEN_TYPE_Operator) {
		return 0;
	}

	if (root->right->token->type == TOKEN_TYPE_Number && root->right->token->number == 100) {
		*rootPointer = root->left;
		root->left = NULL;
		Tree_Delete(root);
	}

	return 0;
}
