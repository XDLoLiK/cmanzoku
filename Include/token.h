#ifndef Man_TOKEN_H
#define Man_TOKEN_H

enum TokenTypes {
    IDENTIFIER = 1,
    STRING     = 2,
    NUMBER     = 3
};

struct Token {
    int type;
    char *value;
};

#define PERCENT     1   // %
#define AMPERSAND   2   // &
#define LPAR        3   // (
#define RPAR        4   // )
#define PLUS        5   // +
#define COMMA       6   // ,
#define MINUS       7   // -
#define DOT         8   // .
#define STAR        9   // *
#define COLON       10  // :
#define SEMICOLON   11  // ;
#define LESS        12  // <
#define GREATER     13  // >
#define ASSIGN      14  // =
#define LSQUAREB    15  // [
#define RSQUAREB    16  // ]
#define LBRACE      17  // {
#define RBRACE      18  // }
#define BITOR       19  // |
#define BITAND      20  // &
#define BITXOR      21  // ^
#define NOTEQUAL    22  // !=
#define EQUAL       23  // ==
#define GREQ        24  // >=
#define LEQ         25  // <=
#define IDENTIFIER

#endif // Man_TOKEN_H
