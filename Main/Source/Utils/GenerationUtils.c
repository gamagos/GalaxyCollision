// (C) Sebastian Fiault 2026
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
#include "PhysicsUtils.h"
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
    int32_t maxX, int32_t maxY, int32_t maxZ, // max
    int32_t minX, int32_t minY, int32_t minZ //min
)
{
    Vector3_Int32 result = {0,0,0};
    result.x = randint32(maxX - minX) + minX;
    result.y = randint32(maxY - minY) + minY;
    result.z = randint32(maxZ - minZ) + minZ;
    bool quit = false;

    do
    {
        quit = mapPointExistenceProbabilityExponential32(result, minX, minY, minZ);
    } 
    while (quit);

    return result;
}

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
    int64_t maxX, int64_t maxY, int64_t maxZ, // max 
    int64_t minX, int64_t minY, int64_t minZ // min
)
{
    Vector3_Int64 result = {0,0,0};
    result.x = randint64(maxX - minX) + minX;
    result.y = randint64(maxY - minY) + minY;
    result.z = randint64(maxZ - minZ) + minZ;
    bool quit = false;

    do
    {
        quit = mapPointExistenceProbabilityExponential64(result, minX, minY, minZ);
    } while (quit);

    return result;
}

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
bool checkStarPositionOverlap32(Star_32 star, Star_32* otherStars, uint32_t amount)// NOSONAR
{
    for (unsigned long long i = 0; i < amount; i++) // Check if stars overlap
    {
        if (otherStars[i].position_Terameters.x == star.position_Terameters.x &&
            otherStars[i].position_Terameters.y == star.position_Terameters.y &&
            otherStars[i].position_Terameters.z == star.position_Terameters.z)
        {
            return false;   
        }
    }

    return true;
}

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
bool checkStarPositionOverlap64(Star_64 star, Star_64* otherStars, uint64_t amount)// NOSONAR
{
    for (unsigned long long i = 0; i < amount; i++) // Check if stars overlap
    {
        if (otherStars[i].position_Kilometers.x == star.position_Kilometers.x &&
            otherStars[i].position_Kilometers.y == star.position_Kilometers.y &&
            otherStars[i].position_Kilometers.z == star.position_Kilometers.z)
        {
            return false;
        }
    }

    return true;
}

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
Vector3_Float32 generateVelocity32(BlackHole_32 parentBlackHole, Star_32 star)
{
    bool quit = false;
    unsigned long distanceFromBlackHole = 0;
    float velocity = 0.0f;
    float velocityNoise = 0.0f;
    Vector3_Float32 starToBlackholeVector = {0};
    Vector3_Float32 velocityDirection = {0};
    Vector3_Float32 velocityDirectionNoise = {0};
    Vector3_Float32 result = {0};

    distanceFromBlackHole = getPointsDistanceInt32(parentBlackHole.position_Terameters, star.position_Terameters);
    velocity = getRequiredOrbitVelocity32((float)distanceFromBlackHole, parentBlackHole.mass_10_BillionQuettagrams);
    // Generation of velocity randomness
    quit = false; // NOSONAR
    do
    {
        velocityNoise = (float)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = mapNumberExistenceProbabilityExponential((unsigned long long)velocityNoise, 0);
    } while (quit);
    velocity *= velocityNoise; // Apply random deviation to velocity
    starToBlackholeVector.x = (float)(parentBlackHole.position_Terameters.x - star.position_Terameters.x);
    starToBlackholeVector.y = (float)(parentBlackHole.position_Terameters.y - star.position_Terameters.y);
    starToBlackholeVector.z = (float)(parentBlackHole.position_Terameters.z - star.position_Terameters.z);
    velocityDirection = Vector3Float32CrossProduct(starToBlackholeVector, parentBlackHole.rotationAxis);
    if (!parentBlackHole.rotatesClockwise) // Make star orbit direction match black hole rotation direction
    {
        velocityDirection.x *= -1;
        velocityDirection.y *= -1;
        velocityDirection.z *= -1;
    }
    quit = false; // NOSONAR
    // Velocity direction randomness generation
    do
    {
        velocityDirectionNoise.x = (float)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = mapNumberExistenceProbabilityExponential((unsigned long long)velocityDirectionNoise.x, 0);
        velocityDirectionNoise.y = (float)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = quit && mapNumberExistenceProbabilityExponential((unsigned long long)velocityDirectionNoise.y, 0);
        velocityDirectionNoise.z = (float)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = quit && mapNumberExistenceProbabilityExponential((unsigned long long)velocityDirectionNoise.z, 0); // Repeat until all positions exist
    } while (quit);
    velocityDirection.x *= velocityDirectionNoise.x;
    velocityDirection.y *= velocityDirectionNoise.y;
    velocityDirection.z *= velocityDirectionNoise.z;
    result = createVectorFromTargetDirectionAndMagnitude_Float32(velocityDirection, velocity);

    return result;
}

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
Vector3_Double64 generateVelocity64(BlackHole_64 parentBlackHole, Star_64 star)
{
    bool quit = false;
    unsigned long long distanceFromBlackHole = 0;
    double velocity = 0.0;
    double velocityNoise = 0.0f;
    Vector3_Double64 starToBlackholeVector = { 0 };
    Vector3_Double64 velocityDirection = { 0 };
    Vector3_Double64 velocityDirectionNoise = { 0 };
    Vector3_Double64 result = { 0 };

    distanceFromBlackHole = getPointsDistanceInt64(parentBlackHole.position_Kilometers, star.position_Kilometers);
    velocity = getRequiredOrbitVelocity64((double)distanceFromBlackHole, parentBlackHole.mass_10_BillionQuettagrams);
    // Generation of velocity randomness
    quit = false; // NOSONAR
    do
    {
        velocityNoise = (double)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = mapNumberExistenceProbabilityExponential((unsigned long long)velocityNoise, 0);
    } while (quit);
    velocity *= velocityNoise; // Apply random deviation to velocity
    starToBlackholeVector.x = (double)(parentBlackHole.position_Kilometers.x - star.position_Kilometers.x);
    starToBlackholeVector.y = (double)(parentBlackHole.position_Kilometers.y - star.position_Kilometers.y);
    starToBlackholeVector.z = (double)(parentBlackHole.position_Kilometers.z - star.position_Kilometers.z);
    velocityDirection = Vector3Double64CrossProduct(starToBlackholeVector, parentBlackHole.rotationAxis);
    if (!parentBlackHole.rotatesClockwise) // Make star orbit direction match black hole rotation direction
    {
        velocityDirection.x *= -1;
        velocityDirection.y *= -1;
        velocityDirection.z *= -1;
    }
    quit = false; // NOSONAR
    // Velocity direction randomness generation
    do
    {
        velocityDirectionNoise.x = (double)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = mapNumberExistenceProbabilityExponential((unsigned long long)velocityDirectionNoise.x, 0);
        velocityDirectionNoise.y = (double)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = quit && mapNumberExistenceProbabilityExponential((unsigned long long)velocityDirectionNoise.y, 0);
        velocityDirectionNoise.z = (double)((rand() % INT_MAX) - (INT_MAX / 2));
        quit = quit && mapNumberExistenceProbabilityExponential((unsigned long long)velocityDirectionNoise.z, 0); // Repeat until all positions exist
    } while (quit);
    velocityDirection.x *= velocityDirectionNoise.x;
    velocityDirection.y *= velocityDirectionNoise.y;
    velocityDirection.z *= velocityDirectionNoise.z;
    result = createVectorFromTargetDirectionAndMagnitude_Double64(velocityDirection, velocity);

    return result;
}

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
Star_32* generateStars32Galaxy(uint32_t amount, BlackHole_32 parentBlackHole)
{
    const int32_t maxX = (~0) / 8;
    const int32_t maxY = (~0) / 8;
    const int32_t maxZ = ( (~0) / 8 ) / 10;
    const int32_t minX = 9'000;
    const int32_t minY = 9'000;
    const int32_t minZ = 9'000;

    float donePercent = 0.0f;
    Star_32* otherStarsTemporary = NULL;
    unsigned long progressIncrement = amount > 10'000 ? 100 : amount / 10'000;

    srand((unsigned int)time(NULL));

    Star_32 *stars = calloc(amount, sizeof(Star_32));
    if (stars == NULL) // check for allocation failure
    {
        fprintf(stderr, 
                "%s %s Memory for Star32 array could not be allocated",
                FATAL_TAG,
                ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    for (unsigned long i = 0; i < amount; i++)
    {
        // ### Position generation ###
        bool positionValid = true;
        unsigned long amountOtherStars = 0;
        while (!positionValid) // Checks for star overlap and does not generate star if a star already exists in that position
        {
            stars[i].position_Terameters = generatePosition32(maxX, maxY, maxZ, minX, minY, minZ);
            amountOtherStars = i - 1;
            otherStarsTemporary = calloc(amountOtherStars, sizeof(Star_32));
            for (unsigned long j = 0; j < amountOtherStars; j++)
            {
                otherStarsTemporary[j] = stars[j];
            }
            positionValid = checkStarPositionOverlap32(stars[i], otherStarsTemporary, amountOtherStars);
            free(otherStarsTemporary);
        }

        // ### Velocity generation ###
        stars[i].velocity_KilometersPerSecond = generateVelocity32(parentBlackHole, stars[i]);

        // ### Mass generation ###
        stars[i].mass_10_BillionQuettagrams = MINIMAL_STAR_MASS_SOLAR_MASSES_FLOAT;
        stars[i].mass_10_BillionQuettagrams *= ((float)randint32(INT32_MAX) / ((float)INT32_MAX / MASS_MOST_MASSIVE_OBSERVED_STAR_QUETTAGRAMS_FLOAT));

        // ### Standard gravitational parameter precomputation ###
        stars[i].standardGravitationalParameter_TerametersCubedPerPetasecondSquared = GRAVITATIONAL_CONSTANT_FLOAT * stars[i].mass_10_BillionQuettagrams;

        // ### Luminosity generation ###
        stars[i].luminosity_Quettalumen = (
            (
                LUMINOSITY_MOST_LUMINOUS_STAR_QUETTALUMEN_FLOAT - LUMINOSITY_SMALLEST_RED_DWARF_QUETTALUMEN_FLOAT)
            * (stars[i].mass_10_BillionQuettagrams / MASS_MOST_MASSIVE_OBSERVED_STAR_QUETTAGRAMS_FLOAT)
            )
            + LUMINOSITY_SMALLEST_RED_DWARF_QUETTALUMEN_FLOAT;

        //TODO ### Color generation ###

        // ### Progress meter ###
        if (i != 0 && progressIncrement % i == 0)
        {
            donePercent = (float)i / (float)amount;
            printf("%lu stars out of %lu generated %6.2f", i, amount, donePercent); // NOSONAR, false alert due to platform specificness of uint32_t
        }
    }

    return stars;
}


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
Star_64* generateStars64Galaxy(uint64_t amount, BlackHole_64 parentBlackHole)
{
    printf("Generating Star64 stars\n");

    const int64_t maxX = (~0) / 8;
    const int64_t maxY = (~0) / 8;
    const int64_t maxZ = ((~0) / 8) / 10;
    const int64_t minX = 9'000;
    const int64_t minY = 9'000;
    const int64_t minZ = 9'000;

    double donePercent = 0.0;
    Star_64* otherStarsTemporary = NULL;
    unsigned long long progressIncrement = amount > 10'000 ? 100 : amount / 10'000;

    srand((unsigned int)time(NULL));

    Star_64 *stars = calloc(amount, sizeof(Star_64));
    if (stars == NULL)
    {
        fprintf(stderr, 
                "%s %s Memory for Star64 array could not be allocated\n",
                FATAL_TAG,
                ERROR_TAG);
        exit(EXIT_FAILURE);
    }

    for (unsigned long long i = 0; i < amount; i++) // TODO split up in smaller functions for readability?
    {
        // ### Position generation ###
        bool positionValid = true;
        unsigned long long amountOtherStars = 0;
        while (!positionValid) // Checks for star overlap and does not generate star if a star already exists in that position
        {
            stars[i].position_Kilometers = generatePosition64(maxX, maxY, maxZ, minX, minY, minZ);
            amountOtherStars = i - 1;
            otherStarsTemporary = calloc(amountOtherStars, sizeof(Star_64));
            for (unsigned long long j = 0; j < amountOtherStars; j++)
            {
                otherStarsTemporary[j] = stars[j];
            }
            positionValid = checkStarPositionOverlap64(stars[i], otherStarsTemporary, amountOtherStars);
            free(otherStarsTemporary);
        }

        // ### Velocity generation ###
        stars[i].velocity_KilometersPerSecond = generateVelocity64(parentBlackHole, stars[i]);

        // ### Mass generation ###
        stars[i].mass_10_BillionQuettagrams = MINIMAL_STAR_MASS_QUETTAGRAM_DOUBLE;
        stars[i].mass_10_BillionQuettagrams *= ( (double)randint64(INT64_MAX) / ((double)INT64_MAX / MASS_MOST_MASSIVE_OBSERVED_STAR_QUETTAGRAMS_FLOAT) );

        // ### Standard gravitational parameter precomputation ###
        stars[i].standardGravitationalParameter_KilometersCubedPerPetasecondSquared = GRAVITATIONAL_CONSTANT_DOUBLE * stars[i].mass_10_BillionQuettagrams;

        // ### Luminosity generation ###
        stars[i].luminosity_Quettalumen = (
            (
                LUMINOSITY_MOST_LUMINOUS_STAR_QUETTALUMEN_DOUBLE - LUMINOSITY_SMALLEST_RED_DWARF_QUETTALUMEN_DOUBLE) 
                * (stars[i].mass_10_BillionQuettagrams / MASS_MOST_MASSIVE_OBSERVED_STAR_QUETTAGRAMS_DOUBLE)
            )
            + LUMINOSITY_SMALLEST_RED_DWARF_QUETTALUMEN_DOUBLE;

        //TODO ### Color generation ###

        // ### Progress meter ###
        if (i != 0 && progressIncrement % i == 0)
        {
            donePercent = (double)i / (double)amount;
            printf("%llu stars out of %llu generated %6.2lf", i, amount, donePercent);
        }
    }

    return stars;
}
