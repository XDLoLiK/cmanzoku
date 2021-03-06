#include "Utils/file.h"

size_t GetFileSize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	return fileSize;
}

char *GetFileExtension(char *fileName)
{
	if (fileName == NULL) {
		return fileName;
	}

	char *dot = strrchr(fileName, '.');
    if (dot == NULL || dot == fileName) {
    	return NULL;
    }
    return dot + 1;
}
