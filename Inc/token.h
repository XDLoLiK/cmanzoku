#ifndef Man_TOKEN_H
#define Man_TOKEN_H

enum Token_Type {
    TOKEN_TYPE_Endmarker   = 0,
    TOKEN_TYPE_Number      = 1,
    TOKEN_TYPE_String      = 2,
    TOKEN_TYPE_Identifier  = 3,
    TOKEN_TYPE_Operator    = 4
};

enum Token_Code {
    TONEN_OP_NotAnOperator = 0
    TOKEN_OP_Add           = 10,
    TOKEN_OP_Sub           = 11,
    TOKEN_OP_Mul           = 12,
    TOKEN_OP_Div           = 13,
    TOKEN_OP_Mod           = 14,
    TOKEN_OP_Bitor         = 15,
    TOKEN_OP_Bitand        = 16,
    TOKEN_OP_Bitxor        = 17,
    TOKEN_OP_Bitnot        = 18,
    TOKEN_OP_Bitshl        = 19,
    TOKEN_OP_Bitshr        = 20,
    TOKEN_OP_Assignment    = 21,
    TOKEN_OP_Lor           = 22,
    TOKEN_OP_Land          = 23,
    TOKEN_OP_Lnot          = 24,
    TOKEN_OP_Equals        = 25,
    TOKEN_OP_Nequals       = 26,
    TOKEN_OP_Less          = 27,
    TOKEN_OP_Greater       = 28,
    TOKEN_OP_Greatereq     = 29,
    TOKEN_OP_Lesseq        = 30,
    TOKEN_OP_Colon         = 31,
    TOKEN_OP_Semicolon     = 32,
    TOKEN_OP_Comma         = 33,
    TOKEN_OP_Dot           = 34,
    TOKEN_OP_Pow           = 35,
    TOKEN_OP_Lsquare       = 36,
    TOKEN_OP_Rsquare       = 37,
    TOKEN_OP_Lround        = 38,
    TOKEN_OP_Rround        = 39,
    TOKEN_OP_Lcurly        = 40,
    TOKEN_OP_Rcurly        = 41,
    TOKEN_OP_AddEqual      = 42,
    TOKEN_OP_SubEqual      = 43,
    TOKEN_OP_MulEqual      = 44,
    TOKEN_OP_DivEqual      = 45,
    TOKEN_OP_NewLine       = 46,
    TOKEN_OP_VarDecl       = 47,
    TOKEN_KW_If            = 100,
    TOKEN_KW_Else          = 101,
    TOKEN_KW_For           = 102,
    TOKEN_KW_While         = 103,
    TOKEN_KW_Return        = 104,
    TOKEN_KW_Continue      = 105,
    TOKEN_KW_Break         = 106,
    TOKEN_KW_Function      = 107,
    TOKEN_KW_Void          = 108
};

struct Token {
    enum Token_Type type;
    union {
        enum Token_Code code;
        int64_t number;
        char *string;
    };
};

Token_Code GetKeyword(const char *tokenString);
Token_Code GetOneCharOperator(char firstSymbol);
Token_Code GetTwoCharOperator(char firstSymbol, char secondSymbol);
Token_Code GetThreeCharOperator(char firstSymbol, char secondSymbol, char thirdSymbol);

#endif // Man_TOKEN_H
