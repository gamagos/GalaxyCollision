// (C) Sebastian Fiault 2026
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../Include/Constants.h"
#include "../../Include/GalaxyCollision.h"
#include "../../Include/Types.h"
#include "../../Include/Utils/DataUtils.h"

char* formatString(const char* toFormat, ...)
{
    va_list args;
    va_start(args, toFormat);
    #ifdef gamagos_OS_IS_WINDOWS //TODO consider defining function _vscprintf and vsprintf_s in GalaxyCollision.h if not defined in std libraries for cleaner code!
        size_t amountCharacters = _vscprintf(toFormat, args) + 1U; //* There needs to be +1 because _vscprintf just doesn't count null-terminators for some reason
    #elif defined(__linux__)
        size_t amountCharacters = vsnprintf(NULL, 0, toFormat, args) + 1U; //* There needs to be +1 because _vscprintf just doesn't count null-terminators for some reason;
    #endif
    char* result = calloc(amountCharacters, sizeof(char));
    if (!result)
    {
        perror("Failed to allocate memory in formatString() in DataUtils.c");
        va_end(args);
        return (char*)NULL;
    }
    #ifdef gamagos_OS_IS_WINDOWS //TODO consider defining function _vscprintf and vsprintf_s in GalaxyCollision.h if not defined in std libraries for cleaner code!
        vsprintf_s(result, amountCharacters * sizeof(char), toFormat, args);
    #elif defined(__linux__)
        va_end(args);
        va_start(args, toFormat);
        vsprintf(result, toFormat, args);
    #endif
    va_end(args);

    return result;
}

void safer_free(void** pointer)
{
    free(*pointer);
    *pointer = 0;
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
            "%s %s Failed to allocate memory for intArray \t in wideRandint() in GeneratorUtils.c",
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

errno_t gamagos_memcpy_s(void* destination, size_t destinationSize, void* source, size_t count)
{
    errno_t success = 0; // 0 means no error
    
    // ============================
    // Some error handling
    // ============================

    // When copying would result in overflow
    if (destinationSize < count)
    {
        perror("You cannot copy buffers that are larger than destinationSize into destination, this would cause an overflow (gamagos_memcpy_s, DataUtils.c)");
        return 1;
    }

    // ----------------------------
    // Check for null pointers
    // ----------------------------
    if (!destination){
        perror("destination cannot be null (gamagos_memcpy_s, DataUtils.c)");
        return 1;
    }
    if (!source){
        perror("source cannot be null (gamagos_memcpy_s, DataUtils.c)");
        return 1;
    }

    // ============================
    // Actual memcpy logic
    // ============================
    #ifdef gamagos_OS_IS_WINDOWS // Uses the safer memcpy_s if on windows
        success = memcpy_s(destination, destinationSize, source, count);
    #elif defined(__linux__) // Uses normal memcpy if on linux
    success = (errno_t)(size_t)memcpy(destination, source, count);
    success = success == 0 ? 1 : 0;
    #endif
    
    return success;
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
        gamagos_memcpy_s(&value, sizeof(value), (char*)narrowInts + readOffset, intSize);
        result = result | value << (i * intSize);
    }
    return result;
}
