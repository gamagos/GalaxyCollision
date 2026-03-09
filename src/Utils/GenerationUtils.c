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
REMARKS:
    DOES NOT ARITHMETICALLY ADD TOGETHER INTS, it just chains the raw bytes and returns them as one int
*/
unsigned long long buildWideIntFromNarrowInts(void *narrowInts, size_t elementSize, unsigned long long elementCount)
{
    if (elementCount * elementSize > 8)
    {
        printf("%s maximum allowed size for narrowInts is 8 bytes!", ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    char bytesPtr = (char*)narrowInts;

    unsigned long long result = 0;
    for (unsigned long long i = 0; i < elementCount; i++)
    {
        unsigned long long value = 0;
        memcpy(&value, bytesPtr + (i * elementSize), elementSize);
        result = result | value << (i * elementSize);
    }
    return result;
}


unsigned long long wideRandint(short int size)
{

}


/*
SYNOPSIS:
    Generates a random int with 32 bits
DESCRIPTION:
    Uses the regular rand() function and adapts the output
    to be 32 bits to avoid platform specific errors where rand() does
    not yield 32 bits
*/
int32_t randint32(int32_t max)
{
    srand((int)time(NULL));

    switch (sizeof(int))
    {
        case 8:
            int32_t result = 0;
            int randomIntsTempory[] = {0};
            short int count = 4; // 4 * 8bits = 32bits
            for (short int i; i < count; i++) randomIntsTempory[i] = rand();
            result = (int32_t)buildWideIntFromNarrowInts(randomIntsTempory, sizeof(int), count);
            return result % max;
        case 16:  
            uint32_t result = 0;
            result = rand() << 16 | rand();
            return result % max;
        case 32:  
            result = rand();
            return result % max;
        case 64:  
            int32_t result = (int32_t)rand();
            return result % max;
    }
}


/*
SYNOPSIS:
    Generates a random int with 64 bits
DESCRIPTION:
    Uses the regular rand() function and adapts the output
    to be 64 bits to avoid platform specific errors where rand() does
    not yield 64 bits
*/
int64_t randint64(int64_t max)
{
    srand(time(NULL));

    switch (sizeof(int))
    {
    case 8:
        uint32_t result = 0;
        result = rand() >> 56 | rand() >> 48 | rand() >> 40 | rand() >> 32 | rand() >> 24 | rand() >> 16 | rand() >> 8 | rand();
        return result % max;
    case 16:
        randomInt64.a16 = rand();
        randomInt64.b16 = rand();
        randomInt64.c16 = rand();
        randomInt64.d16 = rand();
        return randomInt64.a64;
    case 32:
        return rand() % max;
    case 64:
        return rand() % max;
    }
}


/*
SYNOPSIS:
    Generates a Vector3Float32 with random values from -maxValueAbsolute to maxValueAbsolute
    using 32 bit floats
DESCRIPTION:
    Generates a Vector3Float32 with random values in the range from -maxValueAbsolute to maxValueAbsolute
    using 32 bit floats.
    It takes the smallest representable precision across the range from -maxValueAbsolute to maxValueAbsolute
    as the step size for generation.
    It needs to do this to keep distribution even because of how floats work.
*/
Vector3Int32 generatePosition32(int32_t maxX, int32_t maxY, int32_t maxZ)
{
    Vector3Int32 result;
    result.x = randint32(maxX);
    result.y = randint32(maxY);
    result.z = randint32(maxZ);
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
    Vector3Int64 result;
    result.x = randint64(maxX);
    result.y = randint64(maxY);
    result.z = randint64(maxZ);
}


/*
SYNOPSIS:
    Generates an array of StarFloat32 stars around a black hole
DESCRIPTION:
    Generates an array of StarFloat32 stars around a black hole, all with starting velocities,
    positions, colors and other values set up so that the stars form a visually detailed spinning galaxy around
    their parent black hole.
    The generation uses hardcode, predifened paramaters tuned to achieve pretty results.
ARGS:
    amount: The amount of stars to generate as an unsigned 32 bit int
    parentBlackHole: The black hole that the stars will orbit around as a 32 bit precision BlackHoleFloat32
RETURNS:
    An array of StarFloat32 stars, with it's fields adjusted so that all the stars together look like a galaxy
*/
StarFloat32* generateStarsFloat32Galaxy(uint32_t amount, BlackHoleFloat32 parentBlackHole)
{
    const int32_t maxX = ~0;


    StarFloat32 *stars = calloc(amount, sizeof(StarFloat32));
    if (stars == NULL) // check for allocation failir
    {
        fprintf(stderr, "%s %s Memory for StarFloat32[] array could not be allocated", ERROR_TAG, FATAL_TAG);
        exit(EXIT_FAILURE);
    }

    StarFloat32 *tmpPointerStars = stars;
    for (int i = 0; i < amount; i++)
    {

        tmpPointerStars++;
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
StarDouble64* generateStarsDouble64Galaxy(uint64_t amount, BlackHoleDouble64 parentBlackHole)
{
    StarDouble64 *stars = calloc(amount, sizeof(StarDouble64));
    if (stars == NULL)
    {
        fprintf(stderr, "%s %s Memory for StarDouble64[] array could not be allocated", ERROR_TAG, FATAL_TAG);
        exit(EXIT_FAILURE);
    }

    StarDouble64 *tmpPointerStars = stars;
    for (int i = 0; i < amount; i++)
    {
        tmpPointerStars++;
    }
    return stars;
}
