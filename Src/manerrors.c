#include "manerrors.h"

void Man_PrintTokError(struct Tokenizer *tok)
{
	switch (tok->errorCode) {
		case TOK_ERR_NoError:
			return;

		case TOK_ERR_UnsuccessfulAllocation:
			fprintf(stderr, RED("Memory allocation error\n"));
			break;

		case TOK_ERR_AbruptEndOfString:
			fprintf(stderr, RED("ERROR: line %d: Abrupt end of string:\n\t"), tok->lineNumber);
			fwrite(tok->currentToken, sizeof (char), tok->currentToken - tok->bufferCurrent, stderr);
			fprintf(stderr, "\t^");
			for (int i = 1; i < (int)(tok->currentToken - tok->bufferCurrent); i++) {
				fprintf(stderr, "~");
			}
			fprintf(stderr, "\n");
			break;

		case TOK_ERR_AbruptEndOfComment:
			fprintf(stderr, RED("ERROR: line %d: Abrupt end of commentary:\n\t"), tok->lineNumber);
			fwrite(tok->currentToken, sizeof (char), tok->currentToken - tok->bufferCurrent, stderr);
			fprintf(stderr, "\t^");
			for (int i = 1; i < (int)(tok->currentToken - tok->bufferCurrent); i++) {
				fprintf(stderr, "~");
			}
			fprintf(stderr, "\n");
			break;

		case TOK_ERR_ImpossibleOperator:
			fprintf(stderr, RED("ERROR: line %d: There's no such operator:\n\t"), tok->lineNumber);
			fwrite(tok->currentToken, sizeof (char), tok->currentToken - tok->bufferCurrent, stderr);
			fprintf(stderr, "\t^");
			for (int i = 1; i < (int)(tok->currentToken - tok->bufferCurrent); i++) {
				fprintf(stderr, "~");
			}
			fprintf(stderr, "\n");
			break;

		case TOK_ERR_ImpossibleState:
			fprintf(stderr, RED("Tokenizer error\n"));
			break;

		case TOK_ERR_NotExistingFile:
			fprintf(stderr, RED("There's no such file or directory\n"));
			break;

		default:
			fprintf(stderr, RED("Unrecognisable Error\n"));
			break;
	}
	exit(0);
}

void Man_PrintParserError(struct Parser *parser)
{
	switch (parser->errorCode) {
		case PARSER_ERR_NoError:
			return;

		case PARSER_ERR_UnsuccessfulAllocation:
			fprintf(stderr, RED("Memory allocation error\n"));
			break;

		case PARSER_ERR_TokenizerError:
			Man_PrintTokError(parser->tok);
			break;

		default:
			fprintf(stderr, RED("Unrecognisable Error\n"));
			break;
	}
	exit(0);
}
