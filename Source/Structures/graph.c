#include "Structures/graph.h"

int Tree_CreateGraph(struct Tree_Node *root, char const *fileName)
{
	char *dotFileName = calloc(strlen(fileName) + 10, sizeof (char));
	char *pngFileName = calloc(strlen(fileName) + 10, sizeof (char));	
	if (dotFileName == NULL || pngFileName == NULL) {
		free(pngFileName);
		free(dotFileName);
		return 1;
	}
	strcpy(dotFileName, fileName);
	strcat(dotFileName, ".tree.dot");
	strcpy(pngFileName, fileName);
	strcat(pngFileName, ".tree.png");

	FILE *dotFile = fopen(dotFileName, "w");
	if (dotFile == NULL) {
		free(pngFileName);
		free(dotFileName);
		return 1;
	}

	fprintf(dotFile, "digraph tree {\n");
	fprintf(dotFile, "\trankdir=HR;\n");
	fprintf(dotFile, "\tnode [shape=record];\n");

	if (Tree_DumpNodes(root, dotFile)) {
	 	return 1;
	}
	fprintf(dotFile, "}\n");
	fclose(dotFile);

	char *systemCommand = calloc(strlen(dotFileName) + strlen(pngFileName) + 16, sizeof (char));
	if (systemCommand == NULL) {
		free(pngFileName);
		free(dotFileName);
		return 1;
	}
	sprintf(systemCommand, "dot %s -T png -o %s", dotFileName, pngFileName);
	system(systemCommand);

	free(systemCommand);
	free(pngFileName);
	free(dotFileName);
	return 0;
}

int Tree_DumpNodes(struct Tree_Node *root, FILE *dotFile)
{
	if (dotFile == NULL) {
		return 1;
	}

	int ret = 0;
	if (root) {
		ret = Tree_DumpNode(root, dotFile);
	}

	if (root->left) {
		ret = Tree_DumpNodes(root->left, dotFile);
		fprintf(dotFile, "\tnode%p:left -> node%p;\n", root, root->left);
	}

	if (root->right) {
		ret = Tree_DumpNodes(root->right, dotFile);
		fprintf(dotFile, "\tnode%p:right -> node%p;\n", root, root->right);
	}

	return ret;
}

int Tree_DumpNode(struct Tree_Node *root, FILE *dotFile)
{
	if (dotFile == NULL || root == NULL) {
		return 1;
	}

	switch (root->token->type) {
		case TOKEN_TYPE_Endmarker:
			fprintf(dotFile, "\tnode%p [color=yellow, label=\"<left> left | { %s | ", root, "END");
			fprintf(dotFile, "%u } | <right> right\"];\n", root->token->operator);
			break;

		case TOKEN_TYPE_Number:
			fprintf(dotFile, "\tnode%p [color=blue, label=\"<left> left | { %s | ", root, "NUMBER");
			fprintf(dotFile, "%" PRId64 ".%" PRId64 " } | <right> right\"];\n", root->token->number / 100, root->token->number % 100);
			break;

		case TOKEN_TYPE_Operator:
			fprintf(dotFile, "\tnode%p [color=red, label=\"<left> left | { %s | ", root, "OPERATOR");
			char operator[32] = "";
			Tree_OperatorToString(operator, root->token->operator);
			fprintf(dotFile, "%s } | <right> right\"];\n", operator);
			break;

		case TOKEN_TYPE_String:
			fprintf(dotFile, "\tnode%p [color=green, label=\"<left> left | { %s | ", root, "STRING");
			fprintf(dotFile, "%s } | <right> right\"];\n", root->token->string);
			break;

		case TOKEN_TYPE_Identifier:
			fprintf(dotFile, "\tnode%p [color=green, label=\"<left> left | { %s | ", root, "IDENTIFIER");
			fprintf(dotFile, "%s } | <right> right\"];\n", root->token->identifier);
			break;

		default:
			return 1;
	}

	return 0;
}

int Tree_OperatorToString(char *buffer, enum Token_Code operator)
{
	switch (operator) {
		case TOKEN_OP_Add:
			strcpy(buffer, "+");
			break;

		case TOKEN_OP_Sub:
			strcpy(buffer, "-");
			break;

		case TOKEN_OP_Mul:
			strcpy(buffer, "*");
			break;

		case TOKEN_OP_Div:
			strcpy(buffer, "/");
			break;

		case TOKEN_OP_Mod:
			strcpy(buffer, "%");
			break;

		case TOKEN_OP_Bitor:
			strcpy(buffer, "|");
			break;

		case TOKEN_OP_Bitand:
			strcpy(buffer, "&");
			break;

		case TOKEN_OP_Bitxor:
			strcpy(buffer, "^");
			break;

		case TOKEN_OP_Bitnot:
			strcpy(buffer, "~");
			break;

		case TOKEN_OP_Bitshl:
			strcpy(buffer, "<<");
			break;

		case TOKEN_OP_Bitshr:
			strcpy(buffer, ">>");
			break;

		case TOKEN_OP_Assignment:
			strcpy(buffer, "=");
			break;

		case TOKEN_OP_Lor:
			strcpy(buffer, "||");
			break;

		case TOKEN_OP_Land:
			strcpy(buffer, "&&");
			break;

		case TOKEN_OP_Lnot:
			strcpy(buffer, "!");
			break;

		case TOKEN_OP_Equals:
			strcpy(buffer, "==");
			break;

		case TOKEN_OP_Nequals:
			strcpy(buffer, "!=");
			break;

		case TOKEN_OP_Less:
			strcpy(buffer, "\\<");
			break;

		case TOKEN_OP_Greater:
			strcpy(buffer, "\\>");
			break;

		case TOKEN_OP_Greatereq:
			strcpy(buffer, "\\>=");
			break;

		case TOKEN_OP_Lesseq:
			strcpy(buffer, "\\<=");
			break;

		case TOKEN_OP_Colon:
			strcpy(buffer, ":");
			break;

		case TOKEN_OP_Semicolon:
			strcpy(buffer, ";");
			break;

		case TOKEN_OP_Comma:
			strcpy(buffer, ",");
			break;

		case TOKEN_OP_Dot:
			strcpy(buffer, ".");
			break;

		case TOKEN_OP_Pow:
			strcpy(buffer, "**");
			break;

		case TOKEN_OP_Lsquare:
			strcpy(buffer, "[");
			break;

		case TOKEN_OP_Rsquare:
			strcpy(buffer, "]");
			break;

		case TOKEN_OP_Lround:
			strcpy(buffer, "(");
			break;

		case TOKEN_OP_Rround:
			strcpy(buffer, ")");
			break;

		case TOKEN_OP_Lcurly:
			strcpy(buffer, "{");
			break;

		case TOKEN_OP_Rcurly:
			strcpy(buffer, "}");
			break;

		case TOKEN_OP_AddEqual:
			strcpy(buffer, "+=");
			break;

		case TOKEN_OP_SubEqual:
			strcpy(buffer, "-=");
			break;

		case TOKEN_OP_MulEqual:
			strcpy(buffer, "*=");
			break;

		case TOKEN_OP_DivEqual:
			strcpy(buffer, "/=");
			break;

		case TOKEN_OP_ModEqual:
			strcpy(buffer, "%=");
			break;

		case TOKEN_OP_BitorEqual:
			strcpy(buffer, "|=");
			break;

		case TOKEN_OP_BitandEqual:
			strcpy(buffer, "&=");
			break;

		case TOKEN_OP_BitxorEqual:
			strcpy(buffer, "^=");
			break;

		case TOKEN_OP_BitshlEqual:
			strcpy(buffer, "<<=");
			break;

		case TOKEN_OP_BitshrEqual:
			strcpy(buffer, ">>=");
			break;

		case TOKEN_OP_PowEqual:
			strcpy(buffer, "**=");
			break;

		case TOKEN_OP_FunctionCall:
			strcpy(buffer, "call");
			break;

		case TOKEN_OP_GetAddr:
			strcpy(buffer, "addr");
			break;

		case TOKEN_OP_AccessAddr:
			strcpy(buffer, "eval");
			break;

		case TOKEN_OP_Indexate:
			strcpy(buffer, "indexate");
			break;

		case TOKEN_KW_If:
			strcpy(buffer, "if");
			break;

		case TOKEN_KW_Else:
			strcpy(buffer, "else");
			break;

		case TOKEN_KW_For:
			strcpy(buffer, "for");
			break;

		case TOKEN_KW_While:
			strcpy(buffer, "while");
			break;

		case TOKEN_KW_Return:
			strcpy(buffer, "ret");
			break;

		case TOKEN_KW_Continue:
			strcpy(buffer, "next");
			break;

		case TOKEN_KW_Break:
			strcpy(buffer, "stop");
			break;

		case TOKEN_KW_Function:
			strcpy(buffer, "func");
			break;

		case TOKEN_KW_Void:
			strcpy(buffer, "void");
			break;

		case TOKEN_KW_GetAddress:
			strcpy(buffer, "addr");
			break;

		case TOKEN_KW_AtAddress:
			strcpy(buffer, "eval");
			break;

		case TOKEN_KW_False:
			strcpy(buffer, "false");
			break;

		case TOKEN_KW_True:
			strcpy(buffer, "true");
			break;

		case TOKEN_KW_VarDecl:
			strcpy(buffer, "var");
			break;

		default:
			return 1;
	}

	return 0;
}
