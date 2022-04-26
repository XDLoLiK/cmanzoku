#include "Manzoku.h"

#define KEYWORDS_NUMBER 9

static const char *reservedKeywords[KEYWORDS_NUMBER] = {
    "if",
    "else",
    "for",
    "while",
    "ret",
    "continue",
    "break",
    "function",
    "void"
};

Token_Code GetKeyword(const char *tokenString)
{
	for (int kw = 0; kw < KEYWORDS_NUMBER; kw++) {
		if (strcmp(tokenString, reservedKeywords[kw]) == 0) {
			// all ekywords start from 100 and in reservedKeywords
			// array they are in an appropriate order
			return 100 + kw; 
		}
	}
	return TONEN_OP_NotAnOperator;
}

Token_Code GetOneCharOperator(char firstSymbol)
{

}

Token_Code GetTwoCharOperator(char firstSymbol, char secondSymbol)
{

}

Token_Code GetThreeCharOperator(char firstSymbol, char secondSymbol, char thirdSymbol)
{

}
