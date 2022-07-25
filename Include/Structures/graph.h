#ifndef GRAPH_H
#define GRAPH_H

#include "Structures/tree.h"

int Tree_CreateGraph(struct Tree_Node *root, char const *fileName);
int Tree_DumpNodes(struct Tree_Node *root, FILE *dotFile);
int Tree_DumpNode(struct Tree_Node *root, FILE *dotFile);
int Tree_OperatorToString(char *buffer, enum Token_Code operator);

#endif // GRAPH_H

