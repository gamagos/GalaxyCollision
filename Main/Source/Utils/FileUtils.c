// (C) Sebastian Fiault
#include <corecrt.h> // For errno_t
#include <direct.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../GalaxyCollision.h"
#include "DataUtils.h"

char* getAbsolutePath(const char* relativePath)
{
	#if defined(__linux__)
		char* result = calloc(PATH_MAX, sizeof(char));
    #elifdef gamagos_OS_IS_WINDOWS
		char* result = calloc( _MAX_PATH, sizeof(char) );
    #endif
	if (!result)
	{
		perror("Failed to allocate memory for result (FileUtils.c, getAbsolutePath)");
		return NULL;
	}
	result = _fullpath(result, relativePath, _MAX_PATH);
	if (!result)
	{
		perror("Something went wrong with absolute path retrieval (FileUtils.c, getAbsolutePath)");
		return NULL;
	}

	return result;
}

#ifndef CHUNK_SIZE
#define CHUNK_SIZE 512U
#endif
char* readFileAsCharArray(const char* path)
{
	size_t sizeResult = 1024;
	const size_t sizeResultIncrement = 1024;
	FILE* file = 0;
    #ifdef gamagos_OS_IS_WINDOWS
		errno_t failure = fopen_s(&file, path, "r");
    #elif defined(__linux__)
        file = fopen(path, "r");
    #endif
	if (failure)
	{
		perror( formatString("\nFailed to open file \"%s\"", path) );
		return (char*)NULL;
	}
	
	char* result = calloc(sizeResult, sizeof(char));
	if (!result)
	{
		perror("Failed to allocate memory for result (FileUtils.c, readFileAsCharArray())");
		return (char*)NULL;
	}

	char temporaryCharArray[CHUNK_SIZE];
	size_t bytesRead = 0;

	while (fgets(temporaryCharArray, CHUNK_SIZE, file)) // TODO maybe split this up into more functions, getting too nested
	{
		if (sizeResult < bytesRead + CHUNK_SIZE) // Allocate more memory to buffer if buffer is getting to small
		{
			sizeResult += sizeResultIncrement;
			char* temporaryPointer = realloc(result, sizeResult * sizeof(char));
			if (!temporaryPointer)
			{
				perror("Failed to allocate memory for result (FileUtils.c, readFileAsCharArray)");
				return NULL;
			}
			result = temporaryPointer;

			// Set newly allocated memory to 0 to clean garbage in left in memory before allocation
			for (size_t i = sizeResult - sizeResultIncrement - 1; i < sizeResult - 1; i++)
			{
				result[i] = 0;
			}
		}
        #ifdef gamagos_OS_IS_WINDOWS
			memcpy_s(result + bytesRead, sizeResult - bytesRead, temporaryCharArray, CHUNK_SIZE);
		#elif defined(__linux__)
			memcpy(result + bytesRead, temporaryCharArray, CHUNK_SIZE);
        #endif
		bytesRead += strlen(temporaryCharArray);
	}

	fclose(file);
	return result;
}
