// (C) Sebastian Fiault 2026
#include <process.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "../Constants.h"
#include "../Types.h"

/*
* SYNOPSIS
*/
StarFloat32* generateStarsFloat32(uint32_t amount, BlackHoleFloat32 parentBlackHole)
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

StarDouble64* generateStarsDouble64(uint32_t amount, BlackHoleDouble64 parentBlackHole)
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
