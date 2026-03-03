// (C) Sebastian Fiault 2026
#include <process.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "../Constants.h"
#include "../Types.h"

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
StarFloat32* generateStarsFloat32Galaxy(uint32_t amount, BlackHoleFloat32 parentBlackHole)
{
    StarFloat32 *stars = calloc(amount, sizeof(StarFloat32));
    if (stars == NULL)
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
