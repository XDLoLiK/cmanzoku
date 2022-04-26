#ifndef Man_TREE_H
#define Man_TREE_H

#include <stdio.h>
#include <stdlib.h>

struct Tree_Node {
	struct Tree_Node *left;
	struct Tree_Node *right;
	struct Token *token;
};

// Create/delete
struct Tree_Node *Tree_NewNode(struct Token *token);
struct Tree_Node *Tree_Delete(struct Tree_Node *treeHead);

// Upload/download
struct Tree_Node *Tree_Upload(struct Tree_Node *root);
struct Tree_Node *Tree_Download(struct Tree_Node *root);

#endif // Man_TREE_H
