#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "Structures/tree.h"

int Tree_Optimize(struct Tree_Node **rootPointer);
int __Tree_Optimize(struct Tree_Node **rootPointer);

int Tree_OptimizeNullAddition(struct Tree_Node **rootPointer);
int Tree_OptimizeNullSubstraction(struct Tree_Node **rootPointer);
int Tree_OptimizeNullMultiplication(struct Tree_Node **rootPointer);
int Tree_OptimizeNullDivision(struct Tree_Node **rootPointer);
int Tree_OptimizeOneMultiplication(struct Tree_Node **rootPointer);
int Tree_OptimizeOneDivision(struct Tree_Node **rootPointer);

#endif // OPTIMIZER_H 
