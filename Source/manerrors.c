#include "manerrors.h"

void Man_PrintError(struct Token *token, const char *message)
{
	if (message == NULL) {
		abort();
	}

	if (token == NULL) {
		fprintf(stderr, RED("%s\n"), message);
		abort();
	}

	fprintf(stderr, "%s:%d:%d: " RED("error:") " %s\n"
					"  %d | %s\n",
					token->file, token->line, token->column,
					message, token->line, token->context);

	for (int i = 0; i < numlen(token->line) + 3; i++) {
		fprintf(stderr, " ");
	}
	fprintf(stderr, "| ");

	for (int i = 0; i < token->column; i++) {
		fprintf(stderr, " ");
	}
	fprintf(stderr, RED("^"));

	for (int i = 0; i < token->length - 1; i++) {
		fprintf(stderr, RED("~"));
	}
	fprintf(stderr, "\n" );
	exit(1);
}
