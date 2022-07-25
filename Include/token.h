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
    TOKEN_TYPE_Operator    = 4,
};

enum Token_Code {
    // Operators start at 0 and do NOT surpass 100
    TONEN_OP_NotAnOperator = 0,

    // Precedence 1
    TOKEN_OP_Dot           = 1,
    TOKEN_OP_FunctionCall  = 2,
    TOKEN_OP_Indexate      = 3,

    // Precedence 2
    TOKEN_OP_GetAddr       = 4,
    TOKEN_OP_AccessAddr    = 5,
    TOKEN_OP_Bitnot        = 6,
    TOKEN_OP_Lnot          = 7,

    // Precedence 3
    TOKEN_OP_Pow           = 8,

    // Precedence 4
    TOKEN_OP_Mul           = 9,
    TOKEN_OP_Div           = 10,
    TOKEN_OP_Mod           = 11,

    // Precedence 5 
    TOKEN_OP_Add           = 12,
    TOKEN_OP_Sub           = 13,

    // Precedence 6
    TOKEN_OP_Bitshl        = 14,
    TOKEN_OP_Bitshr        = 15,

    // Precedence 7
    TOKEN_OP_Less          = 16,
    TOKEN_OP_Greater       = 17,
    TOKEN_OP_Greatereq     = 18,
    TOKEN_OP_Lesseq        = 19,

    // Precedence 8
    TOKEN_OP_Equals        = 20,
    TOKEN_OP_Nequals       = 21,

    // Precedence 9
    TOKEN_OP_Bitand        = 22,

    // Precedence 10
    TOKEN_OP_Bitxor        = 23,

    // Precedence 11
    TOKEN_OP_Bitor         = 24,

    // Precedence 12
    TOKEN_OP_Land          = 25,

    // Precedence 13
    TOKEN_OP_Lor           = 26,
    
    // Precedence 14
    TOKEN_OP_Assignment    = 27,
    TOKEN_OP_AddEqual      = 28,
    TOKEN_OP_SubEqual      = 29,
    TOKEN_OP_MulEqual      = 30,
    TOKEN_OP_DivEqual      = 31,
    TOKEN_OP_ModEqual      = 32,
    TOKEN_OP_BitorEqual    = 33,
    TOKEN_OP_BitandEqual   = 34,
    TOKEN_OP_BitxorEqual   = 35,
    TOKEN_OP_BitshlEqual   = 36,
    TOKEN_OP_BitshrEqual   = 37,
    TOKEN_OP_PowEqual      = 38,

    // Precedence 15
    TOKEN_OP_Comma         = 39,

    // Extra
    TOKEN_OP_Colon         = 40,
    TOKEN_OP_Semicolon     = 41,
    TOKEN_OP_Lsquare       = 42,
    TOKEN_OP_Rsquare       = 43,
    TOKEN_OP_Lround        = 44,
    TOKEN_OP_Rround        = 45,
    TOKEN_OP_Lcurly        = 46,
    TOKEN_OP_Rcurly        = 47,

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
