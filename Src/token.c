#include "Manzoku.h"

#define KEYWORDS_NUMBER 11

static const char *reservedKeywords[KEYWORDS_NUMBER] = {
    "if",
    "else",
    "for",
    "while",
    "ret",
    "func",
    "stop", // break
    "next", // continue
    "addr", // &
    "eval", // *
    "void"
};

bool IsKeyword(const char *tokenString)
{
    for (int kw = 0; kw < KEYWORDS_NUMBER; kw++) {
        if (strncmp(tokenString, reservedKeywords[kw], strlen(reservedKeywords[kw])) == 0) {
            return true; 
        }
    }
    return false;
}

enum Token_Code GetKeyword(const char *tokenString)
{
    for (int kw = 0; kw < KEYWORDS_NUMBER; kw++) {
        if (strncmp(tokenString, reservedKeywords[kw], strlen(reservedKeywords[kw])) == 0) {
            // all keywords start from 100 and in reservedKeywords
            // array they are placed in an appropriate order
            return 100 + kw;
        }
    }
    return TONEN_OP_NotAnOperator;
}
