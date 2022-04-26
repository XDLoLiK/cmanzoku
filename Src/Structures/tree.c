#include "Structures/tree.h"

struct Tree_Node *Tree_NewNode(struct Token *token)
{
	if (token == NULL) {
		return NULL;
	}
	struct Tree_Node *newNode = calloc(1, sizeof (struct Tree_Node));
	if (newNode == NULL) {
		return NULL;
	}
	newNode->token = token;
	return newNode;
}

struct Tree_Node *Tree_Upload(struct Tree_Node *root)
{
	return root;
}

struct Tree_Node *Tree_Download(struct Tree_Node *root)
{
	return root;
}

struct Tree_Node *Tree_Delete(struct Tree_Node *treeHead)
{
	if (treeHead == NULL) {
		return treeHead;
	}
	treeHead->left  = Tree_Delete(treeHead->left);
	treeHead->right = Tree_Delete(treeHead->right);
	free(treeHead->token);
	free(treeHead);
	return NULL;
}
