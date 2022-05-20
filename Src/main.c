#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Frontend_DumpTokens(const char *fileName);
int Frontend_Main(const char *fileName);
int  Backend_Main(const char *fileName);

int main(int argc, char **argv)
{
	if (argc < 2) {
		return 1;
	}
	if (argc >= 3 && strcmp(argv[2], "-dump-token-map") == 0) {
		Frontend_DumpTokens(argv[1]);
	}
	Frontend_Main(argv[1]);
	Backend_Main(argv[1]);
	return 0;
}