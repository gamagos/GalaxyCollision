// (C) Sebastian Fiault 2026
#ifndef GENERATION_UTILS_H
#define GENERATION_UTILS_H

#include <stdint.h>

#include "../Types.h"

#ifndef isInt(x)
#define isInt(x) _Generic((x), \
    short:     true,     unsigned short:     true \
    int:       true,     unsigned int:       true \
    long:      true,     unsigned long:      true, \
    long long: true,     unsigned long long: true, \
    default:   false \
)
#endif

/*
SYNOPSIS:
    Generates a random int with 32 bits
DESCRIPTION:
    Uses the regular rand() function and adapts the output
    to be 32 bits to avoid platform specific errors where rand() does
    not yield 32 bits
*/
int32_t randint32(int32_t max);

/*
SYNOPSIS:
    Generates a random int with 64 bits
DESCRIPTION:
    Uses the regular rand() function and adapts the output
    to be 64 bits to avoid platform specific errors where rand() does
    not yield 64 bits
*/
int64_t randint64(int64_t max);


/*
SYNOPSIS:
    Generates an array of StarFloat32 stars around a black hole
DESCRIPTION:
    Generates an array of StarFloat32 stars around a black hole, all with starting velocities,
    positions, colors and other values set up so that the stars form a visually detailed spinning galaxy around
    their parent black hole.
ARGS:
    amount: The amount of stars to generate as an unsigned 32 bit int
    parentBlackHole: The black hole that the stars will orbit around as a 32 bit precision BlackHoleFloat32
RETURNS:
    An array of StarFloat32 stars, with it's fields adjusted so that all the stars together look like a galaxy
*/
StarFloat32* generateStarsFloat32Galaxy(uint32_t amount, BlackHoleFloat32 parentBlackHole);

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
StarDouble64* generateStarsDouble64Galaxy(uint64_t amount, BlackHoleDouble64 parentBlackHole);

#endif