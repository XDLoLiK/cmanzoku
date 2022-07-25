#ifndef Man_ERRORS_H
#define Man_ERRORS_H

#include "token.h"

#include <stdio.h>
#include <stdlib.h>

#define RED(str)    "\033[0;31m" str "\033[0m"
#define PURPLE(str) "\033[0;35m" str "\033[0m"
#define YELLOW(str) "\033[0;33m" str "\033[0m"

void Man_PrintError(struct Token *token, const char *message);

#endif // Man_ERRORS_H
