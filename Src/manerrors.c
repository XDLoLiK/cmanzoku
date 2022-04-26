#include "manerrors.h"

void Man_PrintTokError(struct Tokenizer *tok)
{
	switch (tok->errorCode) {

		case TOK_ERR_NoError:
			return;

		case TOK_ERR_UnsuccessfulAllocation:
			fprintf(stderr, RED("Memory allocation error\n"));

		case TOK_ERR_AbruptEndOfString:
			fprintf(stderr, RED("FATAL ERROR: line %d: Abrupt end of string:\n\t"), tok->lineNumber);
			fwrite(tok->currentToken, sizeof (char), tok->currentToken - tok->bufferCurrnet, stderr);
			fprintf(stderr, "\t^");
			for (size_t i = 1; i < tok->currentToken - tok->bufferCurrnet; i++) {
				fprintf(stderr, "~");
			}
			fprintf(stderr, "\n");

		case TOK_ERR_AbruptEndOfComment:
			fprintf(stderr, RED("FATAL ERROR: line %d: Abrupt end of commentary:\n\t"), tok->lineNumber);
			fwrite(tok->currentToken, sizeof (char), tok->currentToken - tok->bufferCurrnet, stderr);
			fprintf(stderr, "\t^");
			for (size_t i = 1; i < tok->currentToken - tok->bufferCurrnet; i++) {
				fprintf(stderr, "~");
			}
			fprintf(stderr, "\n");

		case TOK_ERR_ImpossibleOperator:
			fprintf(stderr, RED("FATAL ERROR: line %d: There's no such operator:\n\t"), tok->lineNumber);
			fwrite(tok->currentToken, sizeof (char), tok->currentToken - tok->bufferCurrnet, stderr);
			fprintf(stderr, "\t^");
			for (size_t i = 1; i < tok->currentToken - tok->bufferCurrnet; i++) {
				fprintf(stderr, "~");
			}
			fprintf(stderr, "\n");

		case TOK_ERR_ImpossibleState:
			fprintf(stderr, RED("Tokenizer error\n"));

		case TOK_ERR_NotExistingFile:
			fprintf(stderr, RED("There's no such file or directory\n"));

		default:
			fprintf(stderr, RED("Unrecognisable Error\n"));
			break;
	}
	return;
}

__attribute__((noreturn)) void Man_PrintParserError(struct Parser *parser)
{
	switch (parser->errorCode) {

		case PARSER_ERR_NoError:
			return;

		case PARSER_ERR_UnsuccessfulAllocation:
			fprintf(stderr, RED("Memory allocation error\n"));

		case PARSER_ERR_TokenizerError:
			__attribute__((fallthrough));

		default:
			fprintf(stderr, RED("Unrecognisable Error\n"));
			break;
	}
	return;
}
