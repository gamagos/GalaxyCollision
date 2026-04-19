// (C) Sebastian Fiault 2026
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../Constants.h"
#include "../Types.h"

/*
SYNOPSIS:
    Takes a list of smaller ints and combines them to bigger int
DESCRIPTION:
    Takes a list of smaller ints and chains them together to form a bigger int from first to last.
    Cannot create ints larger than 8 bytes
ARGS:
    narrowInts:
        Pointer to an array of integers of variable width,
        which is why a void pointer is used here. These are the integers that are to be merged into a single
        unsigned long long.
    intSize:
       How large the ints passed actually are, in Bytes
    intCount:
        The amount of ints in the array that the narrowInts pointer points to
RETURNS:
    The unsigned long long representation of the concatenation of the raw bits of the provided integers.
    If the provided bits are fewer than 64 than the returned value will not be larger than 2^(amount of bits),
    meaning it can be cast to smaller integer types and still fit.
REMARKS:
    DOES NOT ARITHMETICALLY ADD TOGETHER INTS, it just chains the raw bytes and returns them as one int.
    The integers all need to have the same size.
    Cannot create integers beyond 8 bytes (unsigned long long int).
    Concatenates ints from first to last
EXAMPLE:

*/
unsigned long long buildWideIntFromNarrowInts(void* narrowInts, size_t intSize, unsigned long long intCount)
{
    if (intCount * intSize > 8)
    {
        fprintf(stderr,
            "%s %s maximum allowed size for narrowInts is 8 bytes \t buildWideIntFromNarrowInts int GeneratorUtils.c",
            FATAL_TAG,
            ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    unsigned long long result = 0;
    for (unsigned long long i = 0; i < intCount; i++)
    {
        unsigned long long value = 0;
        unsigned long long readOffset = (intSize * i);
        memcpy(&value, (char*)narrowInts + readOffset, intSize);
        result = result | value << (i * intSize);
    }
    return result;
}

/*
SYNOPSIS:
    Generates a random int of the specified size in bytes,
    meaning that the returned int will at most be as large as the largest possible unsigned
    int of the specified size.
DESCRIPTION:
    The function takes the size of the int to be generated in bytes,
    calculates how many bytes of regular ints are needed to create
    ann int of the specified size and then generates random integers
    and concatenates them to be a single unsigned long long which then
    can be cast into smaller types.
ARGS:
    size:
        How large the generated int is going to be in Bytes
RETURNS:
    Returns an random unsigned long long that is maximally as large as
    an unsigned int of the specified size in bytes.
REMARKS:
    Even though the method returns and unsigned long long this long is intended to be castable
    to smaller types
EXAMPLE:
    long long int randomNumber = wideRandint(8); ==> 7435984
    int16_t randomNumberSmol = (int16_t)wideRandint(2); ==> -12089
*/
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

/*
SYNOPSIS:
    Generates a random int with 32 bits
DESCRIPTION:
    Uses the regular rand() function and adapts the output
    to be 32 bits to avoid platform specific errors where rand() does
    not yield 32 bits
ARGS:
    The maximum absolute size of the generated random number
RETURNS:
    A random int32_t
EXAMPLE:
    int32_t myRandInt = randint32(500);
*/
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

/*
SYNOPSIS:
    Generates a random int with 64 bits
DESCRIPTION:
    Uses the regular rand() function and adapts the output
    to be 64 bits to avoid platform specific errors where rand() does
    not yield 64 bits
ARGS:
    The maximum absolute size of the generated random number
RETURNS:
    A random int64_t
EXAMPLE:
    int64_t myRandInt = randint64(9040);
*/
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

/*
SYNOPSIS:
    Casts a Vector3Int32 struct to a Vector3Float32 struct
DESCRIPTION:
    Casts all individual components of a Vector3Int32 struct to floats,
    then builds a Vector3Float32 struct from them and returns the new Vector3Float32 struct
ARGS:
    vectorToConvet:
        The vector to convert
RETURNS:
    The provided Vector3Int32 struct with it's components cast to float, creating a Vector3Float32 struct
EXAMPLE:
    Vector3Int32 myVector = {1,3,4};
    Vector3Float32 = Vector3Int32_To_Vector3Float32(myVector); ==> {1.0f, 3.0f, 4.0f}
*/
Vector3_Float32 Vector3Int32_To_Vector3Float32(Vector3_Int32 vectorToConvert)
{
    Vector3_Float32 result = {0};
    result.x = (float)vectorToConvert.x;
    result.y = (float)vectorToConvert.y;
    result.z = (float)vectorToConvert.z;

    return result;
}

/*
SYNOPSIS:
    Casts a Vector3Int64 struct to a Vector3Double64 struct
DESCRIPTION:
    Casts all individual components of a Vector3Int64 struct to doubles,
    then builds a Vector3Double64 struct from them and returns the new Vector3Float32 struct
ARGS:
    vectorToConvet:
        The vector to convert
RETURNS:
    The provided Vector3Int64 struct with it's components cast to doubles, creating a Vector3Double64 struct
EXAMPLE:
    Vector3Int64 myVector = {1,3,4};
    Vector3Double64 = Vector3Int64_To_Vector3Double64(myVector); ==> {1.0, 3.0, 4.0}
*/
Vector3_Double64 Vector3Int64_To_Vector3Double64(Vector3_Int64 vectorToConvert)
{
    Vector3_Double64 result = { 0 };
    result.x = (double)vectorToConvert.x;
    result.y = (double)vectorToConvert.y;
    result.z = (double)vectorToConvert.z;

    return result;
}
