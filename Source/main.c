#include "Manzoku.h"

#include "frontend.h"
#include "backend.h"

int ProcessFlag(int argc, char **argv)
{
	if (argv == NULL || argc < 2) {
		return 1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		if (strcmp(GetFileExtension(argv[i]), "man") == 0) {
			Frontend_Main(argv[i]);
			Backend_Main(argv[i]);
			continue;		
		}
		ProcessFlag(i, argv);
	}

	return 0;
}