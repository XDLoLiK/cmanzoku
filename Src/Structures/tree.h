#ifndef Man_TREE_H
#define Man_TREE_H

#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_N_TABS(n) {         	\
	for (int i = 0; i < n; i++) { 	\
		fprintf(file, "\t");		\
	}								\
}

struct Tree_Node {
	struct Tree_Node *left;
	struct Tree_Node *right;
	struct Token *token;
};

// Create/delete
struct Tree_Node *Tree_NewNode(struct Token *token);
struct Tree_Node *Tree_Delete(struct Tree_Node *root);

// Upload/download
void Tree_Upload(struct Tree_Node *root, FILE *file, int depth);
struct Tree_Node *Tree_Download(FILE *file);

#endif // Man_TREE_H
