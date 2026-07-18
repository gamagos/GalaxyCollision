// (C) Sebastian Fiault 2026
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "../Types.h"
#include "DataUtils.h"
#include "PhysicsUtils.h"
#include "MathUtils.h"

Vector3_Int32 generatePosition32(
    int32_t maxX, int32_t maxY, int32_t maxZ, // max
    int32_t minX, int32_t minY, int32_t minZ //min
)
{
    Vector3_Int32 result = {0, 0, 0};
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

bool checkStarPositionOverlap32(Star_32 star, Star_32* otherStars, uint32_t amount)// NOSONAR
{
    for (unsigned long long i = 0; i < amount; i++) // Check if stars overlap
    {
        if (otherStars[i].position_Terameters.x == star.position_Terameters.x &&
            otherStars[i].position_Terameters.y == star.position_Terameters.y &&
            otherStars[i].position_Terameters.z == star.position_Terameters.z)
        {
            return true;
        }
    }

    return false;
}

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
    //velocity = getRequiredOrbitVelocity32((float)distanceFromBlackHole, parentBlackHole.mass_10_BillionQuettagrams);
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
    //velocity = getRequiredOrbitVelocity64((double)distanceFromBlackHole, parentBlackHole.mass_10_BillionQuettagrams);
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

Star_32* generateStars32Galaxy(uint32_t amount, BlackHole_32 parentBlackHole)
{
    const int32_t maxX = 22;//
    const int32_t maxY = 22;//
    const int32_t maxZ = 22;// To make the galaxy get a more flat cool shape
    const int32_t minX = 0;
    const int32_t minY = 0;
    const int32_t minZ = 0;

    if (maxX == minX)
    {
        perror("maxX and minX cannot be the same value! (GenerationUtils.c (generateStars32Galaxy))");
        return NULL;
    }
    if (maxY == minY)
    {
        perror("maxY and minY cannot be the same value! (GenerationUtils.c (generateStars32Galaxy))");
        return NULL;
    }
    if (maxZ == minZ)
    {
       perror("maxZ and minZ cannot be the same value! (GenerationUtils.c (generateStars32Galaxy))");
       return NULL;
    }

    Star_32* otherStarsTemporary = NULL;

    srand((unsigned int)time(NULL));

    Star_32* stars = calloc(amount, sizeof(Star_32));
    if (stars == NULL) // check for allocation failure
    {
        perror( formatString("%s %s Memory for Star32 array could not be allocated", FATAL_TAG, ERROR_TAG) );
        return NULL;
    }

    for (unsigned long i = 0; i < amount; i++)
    {
        // ### Position generation ###
        //bool positionValid = false;
        //unsigned long amountOtherStars = 0;
        //while (!positionValid) // Checks for star overlap and does not generate star if a star already exists in that position
        //{
        //    stars[i].position_Terameters = generatePosition32(maxX, maxY, maxZ, minX, minY, minZ);
        //    amountOtherStars = i - 1 <= 0 ? i - 1 : 0;
        //    if (amountOtherStars <= 0) break;
        //    otherStarsTemporary = calloc(amountOtherStars, sizeof(Star_32));
        //    if (!otherStarsTemporary)
        //    {
        //        perror( formatString("\nFailed to allocate %.2lf Kilobytes of memory for otherStarsTemporary (GenerationUtils.c, generateStars32Galaxy())", (amountOtherStars * sizeof(Star_32)) / 1000.0) );
        //        return NULL;
        //    }
        //    for (unsigned long j = 0; j < amountOtherStars - 1; j++)
        //    {
        //        otherStarsTemporary[j] = stars[j];
        //    }
        //    positionValid = !checkStarPositionOverlap32(stars[i], otherStarsTemporary, amountOtherStars);
        //    free(otherStarsTemporary);
        //} //! This is the original code!
        //! This is a temporary bypass!!!!!!!!!!!!!!!
        stars[i].position_Terameters.x = ( (rand() + 1) % (maxX - minX)) + minX; // + 1 to prevent zero devision
        stars[i].position_Terameters.y = ( (rand() + 1) % (maxY - minY)) + minY;
        stars[i].position_Terameters.z = ( (rand() + 1) % (maxZ - minZ)) + minZ;
        stars[i].position_Terameters.x *= (rand() % 2) == 1 ? 1 : -1;// Randomize sign
        stars[i].position_Terameters.y *= (rand() % 2) == 1 ? 1 : -1;
        stars[i].position_Terameters.z *= (rand() % 2) == 1 ? 1 : -1;

        // ### Velocity generation ###
        //! Temporarily skip velocity generation because math is broken, also srand inside it breaks rand here as well //stars[i].velocity_KilometersPerSecond = generateVelocity32(parentBlackHole, stars[i]);
        //! Temporary bypass:
        float tmpMaxGeneratedSpeedDivisor = 2; // bigger ,means slower
        stars[i].velocity_KilometersPerSecond.x = ( ((float)rand() + 1) / (float)RAND_MAX) / tmpMaxGeneratedSpeedDivisor; // + 1 to prevent zero division
        stars[i].velocity_KilometersPerSecond.y = ( ((float)rand() + 1) / (float)RAND_MAX) / tmpMaxGeneratedSpeedDivisor;
        stars[i].velocity_KilometersPerSecond.z = ( ((float)rand() + 1) / (float)RAND_MAX) / tmpMaxGeneratedSpeedDivisor;
        stars[i].velocity_KilometersPerSecond.x *= (rand() % 2) == 1 ? 1 : -1;
        stars[i].velocity_KilometersPerSecond.y *= (rand() % 2) == 1 ? 1 : -1;
        stars[i].velocity_KilometersPerSecond.z *= (rand() % 2) == 1 ? 1 : -1;

        // ### Mass generation ###
        //stars[i].mass_10_BillionQuettagrams = MINIMAL_STAR_MASS_SOLAR_MASSES_FLOAT;
        //stars[i].mass_10_BillionQuettagrams *= ((float)randint32(INT32_MAX) / ((float)INT32_MAX / MASS_MOST_MASSIVE_OBSERVED_STAR_QUETTAGRAMS_FLOAT));
        //! Temporary bypass for debugging. Likely srand() inside randint32 is messing with the position random generation

        // ### Standard gravitational parameter precomputation ###
        stars[i].standardGravitationalParameter_TerametersCubedPerPetasecondSquared = GRAVITATIONAL_CONSTANT_FLOAT * stars[i].mass_10_BillionQuettagrams;

        // ### Luminosity generation ###
        stars[i].luminosity_Quettalumen = (
            (
                LUMINOSITY_MOST_LUMINOUS_STAR_QUETTALUMEN_FLOAT - LUMINOSITY_SMALLEST_RED_DWARF_QUETTALUMEN_FLOAT)
            * (stars[i].mass_10_BillionQuettagrams / MASS_MOST_MASSIVE_OBSERVED_STAR_QUETTAGRAMS_FLOAT)
            )
            + LUMINOSITY_SMALLEST_RED_DWARF_QUETTALUMEN_FLOAT;

        // ### Color generation ###
        stars[i].color.Red = (rand() % 150) + 105;
        stars[i].color.Blue = (rand() % 150) + 105;
        stars[i].color.Green = min(stars[i].color.Red, stars[i].color.Blue);
        // Normalize the value so that at least 1 channel is 255
        uint8_t largestColorValue = 0;
        largestColorValue = largestColorValue < stars[i].color.Red   ? stars[i].color.Red   : largestColorValue;
        largestColorValue = largestColorValue < stars[i].color.Green ? stars[i].color.Green : largestColorValue;
        largestColorValue = largestColorValue < stars[i].color.Blue  ? stars[i].color.Blue  : largestColorValue;
        double percentOf255 = (double)largestColorValue / 255.0;
        stars[i].color.Red   = min( (uint8_t)( (double)stars[i].color.Red   / percentOf255 ), 255 );
        stars[i].color.Green = min( (uint8_t)( (double)stars[i].color.Green / percentOf255 ), 255 );
        stars[i].color.Blue  = min( (uint8_t)( (double)stars[i].color.Blue  / percentOf255 ), 255 );
        stars[i].color.Alpha = 255;
    }
    return stars;
}

Star_64* generateStars64Galaxy(uint64_t amount, BlackHole_64 parentBlackHole)
{
    printf("Generating Star64 stars\n");

    const int64_t maxX = LLONG_MAX / 8;
    const int64_t maxY = LLONG_MAX / 8;
    const int64_t maxZ = (LLONG_MAX / 8) / 10;
    const int64_t minX = 9'000;
    const int64_t minY = 9'000;
    const int64_t minZ = 9'000;
	
	if (maxX == minX) perror("maxX and minX cannot be the same value! (GenerationUtils.c (generateStars32Galaxy))"); return NULL;
    if (maxY == minY) perror("maxY and minY cannot be the same value! (GenerationUtils.c (generateStars32Galaxy))"); return NULL;
    if (maxZ == minZ) perror("maxZ and minZ cannot be the same value! (GenerationUtils.c (generateStars32Galaxy))"); return NULL;

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
    }

    return stars;
}
