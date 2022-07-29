#include "manerrors.h"

void Man_PrintError(struct Token *token, const char *format, ...)
{
	if (format == NULL) {
		fprintf(stderr, RED("error:") " unexpected error\n");
		exit(1);
	}

	char *message = calloc(MAX_MSG_LENGTH, sizeof (char));
	if (message == NULL) {
		fprintf(stderr, RED("error:") " not enough memory\n");
		exit(1);
	}
	va_list args;
	va_start(args, format);
	vsprintf(message, format, args);
	va_end(args);

	if (token == NULL) {
		fprintf(stderr, RED("error:") " %s\n", message);
		free(message);
		exit(1);
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
	free(message);
	exit(1);
}
