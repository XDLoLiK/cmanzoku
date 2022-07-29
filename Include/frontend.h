#ifndef FRONTEND_H
#define FRONTEND_H

#include "Manzoku.h"

#include "parser.h"
#include "astopt.h"

int Frontend_DumpTokens(const char *fileName);
int Frontend_Main(const char *fileName);

#endif // FRONTEND_H
