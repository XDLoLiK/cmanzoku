#include <stdio.h>
#include <stdlib.h>

struct Tree_Node *Frontend_Main(const char *fileName);

int main(int argc, char **argv)
{
	if (argc < 2) {
		return 1;
	}
	if (argc >= 3 && strcmp(argv[2], "-display-token-map") == 0) {
		Frontend_DumpTokens(argv[1]);
	}
	Frontend_Main(argv[1]);
	return 0;
}