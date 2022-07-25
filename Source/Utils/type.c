#include "Utils/type.h"

bool IsDigit(char symbol)
{
	if (symbol <= '9' && symbol >= '0') {
		return true;
	}
	return false;
}

bool IsSpace(char symbol)
{
	if ((symbol == ' ')  || (symbol == '\t') || 
	    (symbol == '\r') || (symbol == '\f') || 
	    (symbol == '\v') || (symbol == '\n')) {
		return true;	
	}
	return false;
}

bool IsIdentifierStart(char symbol)
{
	if ((symbol == '_') || 
		(symbol >= 'a'  &&  symbol <= 'z') ||
        (symbol >= 'A'  &&  symbol <= 'Z')) {
		return true;
	}
	return false;
}

bool IsIdentifierSymbol(char symbol)
{
	if (IsIdentifierStart(symbol) || IsDigit(symbol)) {
		return true;
	}
	return false;
}

bool IsOperatorSymbol(char symbol)
{
	if ((symbol == '*') || (symbol == '/') || (symbol == '+') || (symbol == '-') ||
		(symbol == '(') || (symbol == ')') || (symbol == '[') || (symbol == ']') ||
		(symbol == '{') || (symbol == '}') || (symbol == '<') || (symbol == '>') ||
		(symbol == ';') || (symbol == ':') || (symbol == ',') || (symbol == '.') ||
		(symbol == '^') || (symbol == '~') || (symbol == '|') || (symbol == '&') ||
		(symbol == '=') || (symbol == '%') || (symbol == '$') || (symbol == '!')) {
	  	return true;
	}
	return false;
}

inline bool IsPrecedence1(int operator)
{
	return (operator >= 1 && operator <= 3);
}

inline bool IsPrecedence2(int operator)
{
	return (operator >= 4 && operator <= 7);
}

inline bool IsPrecedence3(int operator)
{
	return (operator == 8);
}

inline bool IsPrecedence4(int operator)
{
	return (operator >= 9 && operator <= 11);
}

inline bool IsPrecedence5(int operator)
{
	return (operator >= 12 && operator <= 13);
}

inline bool IsPrecedence6(int operator)
{
	return (operator >= 14 && operator <= 15);
}

inline bool IsPrecedence7(int operator)
{
	return (operator >= 16 && operator <= 19);
}

inline bool IsPrecedence8(int operator)
{
	return (operator >= 20 && operator <= 21);
}

inline bool IsPrecedence9(int operator)
{
	return (operator == 22);
}

inline bool IsPrecedence10(int operator)
{
	return (operator == 23);
}

inline bool IsPrecedence11(int operator)
{
	return (operator == 24);
}

inline bool IsPrecedence12(int operator)
{
	return (operator == 25);
}

inline bool IsPrecedence13(int operator)
{
	return (operator == 26);
}

inline bool IsPrecedence14(int operator)
{
	return (operator >= 27 && operator <= 38);
}

inline bool IsPrecedence15(int operator)
{
	return (operator == 39);
}
