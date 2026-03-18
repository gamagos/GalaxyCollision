// (C) Sebastian Fiault 2026
#ifndef GENERATION_UTILS_H
#define GENERATION_UTILS_H

#include <math.h>
#include <process.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "../Constants.h"
#include "../Types.h"
#include "DataUtils.h"
#include "MathUtils.h"

/*
SYNOPSIS:
    Generates a Vector3Int32 with random values from -maxX/Y/Z to maxmaxX/Y/Z
    using 32 bit ints
DESCRIPTION:
    Generate a Vector3Int32 and populates it's x,y and z components with random integers
    that fit the number range
ARGS:
    maxX:
        Maximum value the x component can become
    maxY:
        Maximum value the y component can become
    maxZ:
        Maximum value the Z component can become
    minX:
        The minimal value the x component must be
    minY:
        The minimal value the y component must be
    minZ:
        The minimal value the z component must be
RETURNS:
    A Vector3Int32 with random ints in the range from +maxX/Y/Z to -maxX/Y/Z
Example:
    Vector3Int32 randomPointInSpace = generatePosition32(10000, 10000, 10000);
*/
Vector3Int32 generatePosition32(
    int32_t maxX, int32_t maxY, int32_t maxZ, // max
    int32_t minX, int32_t minY, int32_t minZ //min
);

/*
SYNOPSIS:
    Generates a Vector3Int64 with random values from -maxValueAbsolute to maxValueAbsolute
    using 64 bit integers
DESCRIPTION:
    Generates a Vector3Int64 with random values in the range from -maxValueAbsolute to maxValueAbsolute
    using 64 bit floats.
    It takes the smallest representable precision across the range from -maxValueAbsolute to maxValueAbsolute
    as the step size for generation.
    It needs to do this to keep distribution even because of how floats work.
ARGS:
    maxX:
        Maximum value the x component can become
    maxY:
        Maximum value the y component can become
    maxZ:
        Maximum value the Z component can become
    minX:
        The minimal value the x component must be
    minY:
        The minimal value the y component must be
    minZ:
        The minimal value the z component must be
RETURNS:
    A Vector3Int64 with random ints in the range from +maxX/Y/Z to -maxX/Y/Z
Example:
    Vector3Int64 randomPointInSpace = generatePosition32(10000, 10000, 10000);
*/
Vector3Int64 generatePosition64(
    int64_t maxX, int64_t maxY, int64_t maxZ, // max 
    int64_t minX, int64_t minY, int64_t minZ // min
);

/*
SYNOPSIS:
    Generates an array of Star32 stars around a black hole
DESCRIPTION:
    Generates an array of Star32 stars around a black hole, all with starting velocities,
    positions, colors and other values set up so that the stars form a visually detailed spinning galaxy around
    their parent black hole.
    The generation uses hard-coded, predefined parameters tuned to achieve pretty results.
ARGS:
    amount: The amount of stars to generate as an unsigned 32 bit int
    parentBlackHole: The black hole that the stars will orbit around as a BlackHole32
RETURNS:
    An array of Star32 stars, with it's fields adjusted so that all the stars, when rendered together
    look like a galaxy
EXAMPLE:
    Star32* starsGalaxy1 = generateStars32Galaxy(20'000, Messier_31);
*/
Star32* generateStars32Galaxy(uint32_t amount, BlackHole32 parentBlackHole);

/*
SYNOPSIS:
    Generates an array of Star64 stars around a parent black hole
DESCRIPTION:
    Generates an array of Star64 stars around a black hole, all with starting velocities,
    positions, colors and other values set up so that the stars form a visually detailed spinning galaxy around
    their parent black hole.
ARGS:
    amount: The amount of stars to generate as an unsigned 64 bit int
    parentBlackHole: The black hole that the stars will orbit around as a BlackHole64
RETURNS:
    An array of Star64 stars, with it's fields adjusted so that all the stars together look like a galaxy
EXAMPLE:
    Star64* starsGalaxyA = generateStars(25'000, sagitatiusA);
*/
Star64* generateStars64Galaxy(uint64_t amount, BlackHole64 parentBlackHole);

#endif