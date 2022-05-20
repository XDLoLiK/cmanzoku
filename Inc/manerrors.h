#ifndef Man_ERRORS_H
#define Man_ERRORS_H

#include "tokenizer.h"
#include "parser.h"
#include "compiler.h"

#define RED(str)    "\033[0;31m" str "\033[0m"
#define PURPLE(str) "\033[0;35m" str "\033[0m"
#define YELLOW(str) "\033[0;33m" str "\033[0m"

void Man_PrintTokError(struct Tokenizer *tok);
void Man_PrintParserError(struct Parser *parser);
void Man_PrintCompilerError(struct Compiler *compiler);

#endif // Man_ERRORS_H
