#include "Manzoku.h"
#include "tokenizer.h"

struct ManToken *Tokenize(FILE* inputFile)
{
	TokenizerState currentState = STATE_StartOfData;

	while (currentState != STATE_EndOfData &&
		   currentState != STATE_TokenizerError) {
		char currentSymbol = fgetc(inputFile);

		if (currentSymbol == EOF) {
			
		}
		else {
			switch (currentState) {

			}
		}
	}

	return NULL;
}
