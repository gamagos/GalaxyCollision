// (C) Sebastian Fiault 2026
#ifndef GENERATION_UTILS_H
#define GENERATION_UTILS_H

#include <stdint.h>

#include "../Types.h"


/* 
SYNOPSIS:
    A union to convert int64s to 8 int8s and everything in between
*/
typedef union int64Conversion
{
    struct // 8 bit
    {
        int8_t a8;
        int8_t b8;
        int8_t c8;
        int8_t d8;
        int8_t e8;
        int8_t f8;
        int8_t g8;
        int8_t h8;
    };
    struct // 16 bit
    {
        int16_t a16;
        int16_t b16;
        int16_t c16;
        int16_t d16;
    };
    struct // 32 bit
    {
        int32_t a32;
        int32_t b32;
    };
    struct // 64 bit
    {
        int64_t a64;
    };
} intConversion;

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