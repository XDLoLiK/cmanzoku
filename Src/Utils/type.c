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
