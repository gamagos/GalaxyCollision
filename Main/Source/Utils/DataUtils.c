    // (C) Sebastian Fiault 2026
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "../Types.h"
#include "DataUtils.h"

char* formatString(const char* toFormat, ...)
{
    va_list args;
    va_start(args, toFormat);
    size_t amountCharacters = _vscprintf(toFormat, args) + 1U; //! There needs to be +1 because _vscprintf just doesn't count null-terminators for some reason
    char* result = calloc(amountCharacters, sizeof(char));
    if (!result)
    {
        perror("Failed to allocate memory in formatString() in DataUtils.c");
        va_end(args);
        return (char*)NULL;
    }
    vsprintf_s(result, amountCharacters * sizeof(char), toFormat, args);
    va_end(args);

    return result;
}

void safer_free(void** pointer)
{
    free(*pointer);
    *pointer = 0;
}

unsigned long long buildWideIntFromNarrowInts(void* narrowInts, size_t intSize, unsigned long long intCount)
{
    if (intCount * intSize > 8)
    {
        perror( formatString("%s %s maximum allowed size for narrowInts is 8 bytes \t buildWideIntFromNarrowInts int GeneratorUtils.c", FATAL_TAG, ERROR_TAG) );
        exit(EXIT_FAILURE);
        return (unsigned long long)NULL;
    }

    unsigned long long result = 0;
    for (unsigned long long i = 0; i < intCount; i++)
    {
        unsigned long long value = 0;
        unsigned long long readOffset = (intSize * i);
        memcpy_s(&value, sizeof(value), (char*)narrowInts + readOffset, intSize);
        result = result | value << (i * intSize);
    }
    return result;
}

unsigned long long wideRandint(short int size)
{
    if (8 % sizeof(int) != 0)
    {
        fprintf(stderr,
            "%s %s Cannot fit ints of size %d in 8 byte unsigned long long. \t wideRandint() in GenerationUtils.c",
            FATAL_TAG,
            ERROR_TAG,
            size);
        exit(EXIT_FAILURE);
    }

    srand((unsigned int)time(NULL));

    short unsigned int amountInts = 8 / sizeof(int);
    int* intArray = calloc(amountInts, sizeof(int));

    if (intArray == NULL)
    {
        fprintf(stderr,
            "%s %s Failed to allocate memory for intArray \t in wideRanint() in GeneratorUtils.c",
            FATAL_TAG,
            ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    for (short unsigned int i = 0; i < amountInts; i++)
    {
        intArray[i] = rand();
    }

    unsigned long long result = buildWideIntFromNarrowInts(intArray, sizeof(int), amountInts);
    free(intArray);

    if (size < 8) // skip size mapping if size is 8 bytes because unsigned long long is already 8 bytes
    {
        result = result % (1ULL << (size * 8)); // Match result to number max number range if not already mapped
    }

    return result;
}

int32_t randint32(int32_t max)
{
    if (max == 0)
    {
        fprintf(stderr,
            "%s %s max can't be 0 \t randint32 in GeneratorUtils.c",
            FATAL_TAG,
            ERROR_TAG);
        exit(EXIT_FAILURE);
    }
    int32_t result = (int32_t)wideRandint(sizeof(int32_t));
    return result % max;
}

int64_t randint64(int64_t max)
{
    if (max == 0)
    {
        fprintf(stderr,
            "%s %s max can't be 0 \t randint32 in GeneratorUtils.c",
            FATAL_TAG,
            ERROR_TAG);
        exit(EXIT_FAILURE);
    }
    int32_t result = (int32_t)wideRandint(sizeof(int32_t));
    return result % max;
}

Vector3_Float32 Vector3Int32_To_Vector3Float32(Vector3_Int32 vectorToConvert)
{
    Vector3_Float32 result = {0};
    result.x = (float)vectorToConvert.x;
    result.y = (float)vectorToConvert.y;
    result.z = (float)vectorToConvert.z;

    return result;
}

Vector3_Double64 Vector3Int64_To_Vector3Double64(Vector3_Int64 vectorToConvert)
{
    Vector3_Double64 result = { 0 };
    result.x = (double)vectorToConvert.x;
    result.y = (double)vectorToConvert.y;
    result.z = (double)vectorToConvert.z;

    return result;
}
