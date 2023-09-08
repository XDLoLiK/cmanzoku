#include "frontend.h"
#include "backend.h"

extern int OPTIMIZATION_LVL;
extern int DEBUG_LVL;

extern int DUMP_TOKENS_FLAG;
extern int DUMP_AST_FLAG;
extern int CREATE_LISTING_FLAG;

int ProcessFlag(char *flag)
{
	if (flag == NULL) {
		return 1;
	}

	if (strncmp(flag, "-dump-ast", 9) == 0) {
		DUMP_AST_FLAG = 1;
	}
	else if (strncmp(flag, "-dump-tokens", 12) == 0) {
		DUMP_TOKENS_FLAG = 1;
	}
	else if (strncmp(flag, "-l", 2) == 0) {
		CREATE_LISTING_FLAG = 1;
	}
	else if (strncmp(flag, "-o", 2) == 0) {
		OPTIMIZATION_LVL = *(flag + 2) - '0';
	}
	else if (strncmp(flag, "-d", 2) == 0) {
		DEBUG_LVL = *(flag + 2) - '0';
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		if (*argv[i] == '-') {
			if (ProcessFlag(argv[i])) {
				Man_PrintError(NULL, "invalid command line option \'%s\'", argv[i]);
			}
		}
	}

	for (int i = 1; i < argc; i++) {
		char *fileExtension = GetFileExtension(argv[i]);
		if (fileExtension == NULL) {
			continue;
		}

		if (strncmp(fileExtension, "man", 3) == 0) {
			Frontend_Main(argv[i]);
			Backend_Main(argv[i]);
		}
		else {
			Man_PrintError(NULL, "invalid file format \'%s\'", argv[i]);
		}
	}

	return 0;
}
