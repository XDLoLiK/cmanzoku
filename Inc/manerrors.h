#ifndef Man_ERRORS_H
#define Man_ERRORS_H

#include "tokenizer.h"
#include "parser.h"

#define RED(str) "\033" #str "m"

void Man_PrintTokError(struct Tokenizer *tok);
void Man_PrintParserError(struct Parser *parser);

#endif // Man_ERRORS_H
