#ifndef Man_FILE_H
#define Man_FILE_H

#include <stdio.h>
#include <string.h>

size_t GetFileSize(FILE *file);
char const *GetFileExtension(char const *fileName);

#endif // Man_FILE_H
