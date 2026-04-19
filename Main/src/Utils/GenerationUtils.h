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
    using 32 bit ints and maps their distribution to e^(-x) using minX/Y/Z as the threshold for 100% existence
ARGS:
    maxX:
        Maximum value the x component can be
    maxY:
        Maximum value the y component can be
    maxZ:
        Maximum value the Z component can be
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
Vector3_Int32 generatePosition32(
    int32_t maxX, int32_t maxY, int32_t maxZ,
    int32_t minX, int32_t minY, int32_t minZ
);

/*
SYNOPSIS:
    Generates a Vector3Int64 with random values from -maxValueAbsolute to maxValueAbsolute
    using 64 bit integers and maps their distribution to e^(-x) using minX/Y/Z as the threshold for 100% existence probability
ARGS:
    maxX:
        Maximum value the x component can be
    maxY:
        Maximum value the y component can be
    maxZ:
        Maximum value the Z component can be
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
Vector3_Int64 generatePosition64(
    int64_t maxX, int64_t maxY, int64_t maxZ, 
    int64_t minX, int64_t minY, int64_t minZ
);

/*
SYNOPSIS:
    Checks wether a star's position overlaps with the of any of the other stars in the otherStars array
ARGS:
    star:
        The star to check overlap for
    otherStars:
        The stars to check the star against for overlap
    amount:
        The amount of stars in the otherStars array
RETURNS:
    Wether the star overlaps with any of the stars in the array
REMARKS:
    Ensure that the Array otherStars does not contain star when calling this method
*/
bool checkStarPositionOverlap32(Star_32 star, Star_32* otherStars, uint32_t amount);

/*
SYNOPSIS:
    Checks wether a star's position overlaps with the of any of the other stars in the otherStars array
ARGS:
    star:
        The star to check overlap for
    otherStars:
        The stars to check the star against for overlap
    amount:
        The amount of stars in the otherStars array
RETURNS:
    Wether the star overlaps with any of the stars in the array
REMARKS:
    Ensure that the Array otherStars does not contain star when calling this method
*/
bool checkStarPositionOverlap64(Star_64 star, Star_64* otherStars, uint64_t amount);

/*
SYNOPSIS:
    Generates the velocity for a star that orbits a black hole
DESCRIPTION:
    Generates a velocity vector for a star that orbits a black hole
    in a galaxy. It generates the velocity for a circular orbit and then applies some randomness
    to it and it's direction.
    It does this by first using a formula to determine the required velocity.
    Next it applies some noise to the result.
    Then it gets the cross-product of the vector that points from the black hole to the star and the rotation axis of the black hole
    and inverts it if the rotation is supposed to be counter-clockwise
    and then applies some noise to that vector.
    The direction of clockwise rotation is determined by looking downwards on the black hole from the rotation axis vector
    so that the rotation axis vector is pointing at you.
ARGS:
    parentBlackHole:
        The black hole the star is orbiting around
    star:
        The star that is orbiting around the black hole
RETURNS:
    The velocity as a vector the star needs to orbit the black hole,
    with respect to the black holes rotation direction and with some random noise applied to it.
REMARKS:
    Note that the rotation direction is determined looking down on the black hole so that the rotation axis vector is pointing at you.
EXAMPLE:
    BlackHole32 parentBlackHole = { .rotatesClockwise = true,
                                    .position = {0},
                                    .rotationAxis = {1.0f, 0.0f, 0.0f} };
    Star32 star = { .position = {2000.0, 500.0, 4000.0} };
    star.velocityKilometersPerSecond = generateVelocity32(parentBlackHole, star);
*/
Vector3_Float32 generateVelocity32(BlackHole_32 parentBlackHole, Star_32 star);

/*
SYNOPSIS:
    Generates the velocity for a star that orbits a black hole
DESCRIPTION:
    Generates a velocity vector for a star that orbits a black hole
    in a galaxy. It generates the velocity for a circular orbit and then applies some randomness
    to it and it's direction.
    It does this by first using a formula to determine the required velocity.
    Next it applies some noise to the result.
    Then it gets the cross-product of the vector that points from the black hole to the star and the rotation axis of the black hole
    and inverts it if the rotation is supposed to be counter-clockwise
    and then applies some noise to that vector.
    The direction of clockwise rotation is determined by looking downwards on the black hole from the rotation axis vector
    so that the rotation axis vector is pointing at you.
ARGS:
    parentBlackHole:
        The black hole the star is orbiting around
    star:
        The star that is orbiting around the black hole
RETURNS:
    The velocity as a vector the star needs to orbit the black hole,
    with respect to the black holes rotation direction and with some random noise applied to it.
REMARKS:
    Note that the rotation direction is determined looking down on the black hole so that the rotation axis vector is pointing at you.
EXAMPLE:
    BlackHole64 parentBlackHole = { .rotatesClockwise = true,
                                    .position = {0},
                                    .rotationAxis = {1.0, 0.0, 0.0} };
    Star64 star = { .position = {2000.0, 500.0, 4000.0} };
    star.velocityKilometersPerSecond = generateVelocity32(parentBlackHole, star);
*/
Vector3_Double64 generateVelocity64(BlackHole_64 parentBlackHole, Star_64 star);

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
Star_32* generateStars32Galaxy(uint32_t amount, BlackHole_32 parentBlackHole);

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
Star_64* generateStars64Galaxy(uint64_t amount, BlackHole_64 parentBlackHole);

#endif