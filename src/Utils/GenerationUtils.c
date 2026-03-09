// (C) Sebastian Fiault 2026
#include <math.h>
#include <process.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#include "../Constants.h"
#include "GenerationUtils.h"
#include "../Types.h"

// TODO split up some of this files methods to other files as this file contains functions used in generation,
// TODO but that do not generate anything themselves


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
unsigned long long buildWideIntFromNarrowInts(void *narrowInts, size_t intSize, unsigned long long intCount)
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
        result = result % ( 1ULL << (size * 8) ); // Match result to number max number range if not already mapped
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
    int32_t result = (int32_t)wideRandint(sizeof(int32_t));
    return result % max;
}


/*
SYNOPSIS:
    Generates a Vector3Int32 with random values from -maxX/Y/Z to maxmaxX/Y/Z
    using 32 bit ints
DESCRIPTION:
    Generate a Vector3Int32 and populates it's x,y and z components with random intgers
    that fit the number range
ARGS:
    maxX:
        Maximum value the x component can become
    maxY:
        Maximum value the y component can become
    maxZ:
        Maximum value the Z component can become
RETURNS:
    A 32 bit vector3 with random ints in the range from +maxX/Y/Z to -maxX/Y/Z
Example:
    Vector3 randomPointInSpace = generatePosition32(10000, 10000, 10000);
*/
Vector3Int32 generatePosition32(int32_t maxX, int32_t maxY, int32_t maxZ)
{
    Vector3Int32 result = {0,0,0};
    result.x = randint32(maxX);
    result.y = randint32(maxY);
    result.z = randint32(maxZ);
    return result;
}

/*
SYNOPSIS:
    Generates a Vector3Float64 with random values from -maxValueAbsolute to maxValueAbsolute
    using 64 bit floats
DESCRIPTION:
    Generates a Vector3Float64 with random values in the range from -maxValueAbsolute to maxValueAbsolute
    using 64 bit floats.
    It takes the smallest representable precision across the range from -maxValueAbsolute to maxValueAbsolute
    as the step size for generation.
    It needs to do this to keep distribution even because of how floats work.
*/
Vector3Int64 generatePosition64(int64_t maxX, int64_t maxY, int64_t maxZ)
{
    Vector3Int64 result = {0,0,0};
    result.x = randint64(maxX);
    result.y = randint64(maxY);
    result.z = randint64(maxZ);
    return result;
}


/*
SYNOPSIS:
    Generates an array of StarFloat32 stars around a black hole
DESCRIPTION:
    Generates an array of StarFloat32 stars around a black hole, all with starting velocities,
    positions, colors and other values set up so that the stars form a visually detailed spinning galaxy around
    their parent black hole.
    The generation uses hard-coded, predefined parameters tuned to achieve pretty results.
ARGS:
    amount: The amount of stars to generate as an unsigned 32 bit int
    parentBlackHole: The black hole that the stars will orbit around as a 32 bit precision BlackHoleFloat32
RETURNS:
    An array of StarFloat32 stars, with it's fields adjusted so that all the stars together look like a galaxy
*/
Star32* generateStarsFloat32Galaxy(uint32_t amount, BlackHole32 parentBlackHole)
{
    const int32_t maxX = (~0) / 8;
    const int32_t maxY = (~0) / 8;
    const int32_t maxZ = ( (~0) / 8 ) / 10;

    Star32 *stars = calloc(amount, sizeof(Star32));
    if (stars == NULL) // check for allocation failure
    {
        fprintf(stderr, 
                "%s %s Memory for Star32[] array could not be allocated",
                FATAL_TAG,
                ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    for (unsigned long i = 0; i < amount; i++)
    {
        stars[i].position = generatePosition32(maxX, maxY, maxZ);
    }

    return stars;
}


/*
SYNOPSIS:
    Generates an array of StarDouble64 stars around a black hole
DESCRIPTION:
    Generates an array of StarDouble64 stars around a black hole, all with starting velocities,
    positions, colors and other values set up so that the stars form a visually detailed spinning galaxy around
    their parent black hole.
ARGS:
    amount: The amount of stars to generate as an unsigned 64 bit int
    parentBlackHole: The black hole that the stars will orbit around as a 64 bit precision BlackHoleFloat64
RETURNS:
    An array of StarFloat64 stars, with it's fields adjusted so that all the stars together look like a galaxy
*/
Star64* generateStarsDouble64Galaxy(uint64_t amount, BlackHole64 parentBlackHole)
{
    const int64_t maxX = (~0) / 8;
    const int64_t maxY = (~0) / 8;
    const int64_t maxZ = ((~0) / 8) / 10;

    Star64 *stars = calloc(amount, sizeof(Star64));
    if (stars == NULL)
    {
        fprintf(stderr, 
                "%s %s Memory for Star64[] array could not be allocated",
                FATAL_TAG,
                ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    for (unsigned long long i = 0; i < amount; i++)
    {
        stars[i].position = generatePosition64(maxX, maxY, maxZ);
    }
    return stars;
}
