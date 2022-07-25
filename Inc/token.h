#ifndef Man_TOKEN_H
#define Man_TOKEN_H

#include "Utils/maths.h"

#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

#define MAX_IDENTIFIER_LENGTH 256

enum Token_Type {
    TOKEN_TYPE_Endmarker   = 0,
    TOKEN_TYPE_Number      = 1,
    TOKEN_TYPE_String      = 2,
    TOKEN_TYPE_Identifier  = 3,
    TOKEN_TYPE_Operator    = 4
};

enum Token_Code {
    // Operators start at 10 and do NOT surpass 100
    TONEN_OP_NotAnOperator = 0,

    // Precedence 1
    TOKEN_OP_Dot           = 34,
    TOKEN_OP_FunctionCall  = 53,
    TOKEN_OP_Indexate      = 54,

    // Precedence 2
    TOKEN_OP_GetAddr       = 55,
    TOKEN_OP_AccessAddr    = 56,
    TOKEN_OP_Bitnot        = 18,
    TOKEN_OP_Lnot          = 24,

    // Precedence 3
    TOKEN_OP_Pow           = 35,

    // Precedence 4
    TOKEN_OP_Mul           = 12,
    TOKEN_OP_Div           = 13,
    TOKEN_OP_Mod           = 14,

    // Precedence 5 
    TOKEN_OP_Add           = 10,
    TOKEN_OP_Sub           = 11,

    // Precedence 6
    TOKEN_OP_Bitshl        = 19,
    TOKEN_OP_Bitshr        = 20,

    // Precedence 7
    TOKEN_OP_Less          = 27,
    TOKEN_OP_Greater       = 28,
    TOKEN_OP_Greatereq     = 29,
    TOKEN_OP_Lesseq        = 30,

    // Precedence 8
    TOKEN_OP_Equals        = 25,
    TOKEN_OP_Nequals       = 26,

    // Precedence 9
    TOKEN_OP_Bitand        = 16,

    // Precedence 10
    TOKEN_OP_Bitxor        = 17,

    // Precedence 11
    TOKEN_OP_Bitor         = 15,

    // Precedence 12
    TOKEN_OP_Land          = 23,

    // Precedence 13
    TOKEN_OP_Lor           = 22,
    
    // Precedence 14
    TOKEN_OP_Assignment    = 21,
    TOKEN_OP_AddEqual      = 42,
    TOKEN_OP_SubEqual      = 43,
    TOKEN_OP_MulEqual      = 44,
    TOKEN_OP_DivEqual      = 45,
    TOKEN_OP_ModEqual      = 46,
    TOKEN_OP_BitorEqual    = 47,
    TOKEN_OP_BitandEqual   = 48,
    TOKEN_OP_BitxorEqual   = 49,
    TOKEN_OP_BitshlEqual   = 50,
    TOKEN_OP_BitshrEqual   = 51,
    TOKEN_OP_PowEqual      = 52,

    // Precedence 15
    TOKEN_OP_Comma         = 33,

    // Extra
    TOKEN_OP_Colon         = 31,
    TOKEN_OP_Semicolon     = 32,
    TOKEN_OP_Lsquare       = 36,
    TOKEN_OP_Rsquare       = 37,
    TOKEN_OP_Lround        = 38,
    TOKEN_OP_Rround        = 39,
    TOKEN_OP_Lcurly        = 40,
    TOKEN_OP_Rcurly        = 41,

    // Keywords start at 100 (important)
    TOKEN_KW_If            = 100,
    TOKEN_KW_Else          = 101,
    TOKEN_KW_For           = 102,
    TOKEN_KW_While         = 103,
    TOKEN_KW_Return        = 104,
    TOKEN_KW_Continue      = 105,
    TOKEN_KW_Break         = 106,
    TOKEN_KW_Function      = 107,
    TOKEN_KW_Void          = 108,
    TOKEN_KW_GetAddress    = 109,
    TOKEN_KW_AtAddress     = 110,
    TOKEN_KW_False         = 111,
    TOKEN_KW_True          = 112,
    TOKEN_KW_VarDecl       = 113,
};

struct Token {
    // type and value
    enum Token_Type type;
    union {
        enum Token_Code operator;
        int64_t number;
        char *string;
        char *identifier;
    };

    // debug info
    char *context;
    int line;
    int column;
    int length;
};

bool IsKeyword(const char *tokenString, int keywordLength);
enum Token_Code GetKeyword(const char *tokenString, int keywordLength);

#endif // Man_TOKEN_H
