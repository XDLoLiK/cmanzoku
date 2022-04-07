#ifndef TOKENIZER_H
#define TOKENIZER_

enum TokenizerState {
	STATE_TokenizerError,
	STATE_StartOfData,
	STATE_EndOfData,
	STATE_String,
	STATE_Number,
	STATE_Operator
};

#endif // TOKENIZER_H
