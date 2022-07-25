#ifndef Man_TYPE_H
#define Man_TYPE_H

#include <stdbool.h>

// Token types
bool IsDigit(char symbol);
bool IsSpace(char symbol);
bool IsIdentifierStart(char symbol);
bool IsIdentifierSymbol(char symbol);
bool IsOperatorSymbol(char symbol);

// Precedence types
bool  IsPrecedence1(int operator);
bool  IsPrecedence2(int operator);
bool  IsPrecedence3(int operator);
bool  IsPrecedence4(int operator);
bool  IsPrecedence5(int operator);
bool  IsPrecedence6(int operator);
bool  IsPrecedence7(int operator);
bool  IsPrecedence8(int operator);
bool  IsPrecedence9(int operator);
bool IsPrecedence10(int operator);
bool IsPrecedence11(int operator);
bool IsPrecedence12(int operator);
bool IsPrecedence13(int operator);
bool IsPrecedence14(int operator);
bool IsPrecedence15(int operator);

#endif // Man_TYPE_H
