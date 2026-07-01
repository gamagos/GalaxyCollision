// (C) Sebastian Fiault 2026
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "../Types.h"
#include "DataUtils.h"
#include "MathUtils.h"

double logCustomBase(double base, double _X)
{
	return log(_X) / log(base);
}

double nthRoot(long long n, double number)
{
	if (number < 0) 
	{
		fprintf(stderr,
				"%s %s cannot get root of negative number \t nthRoot in MathUtils.c",
				FATAL_TAG,
				ERROR_TAG);
		exit(EXIT_FAILURE);
	}
	return pow( number, 1.0 / (double)n );
}

bool mapNumberExistenceProbabilityExponential(unsigned long long number, unsigned long long threshold100Percent)
{
	if (threshold100Percent == 0) // This is a lazy and mathematically incorrect workaround
	{                             //TODO fix this some time!
		threshold100Percent = 1;
	}
	srand((unsigned int)time(NULL));
	// Map the probability of the numbers existence to e^(-x) and normalize it for one x to be 100%
	long double existenceProbability = exp( -(double)number ) / exp( -(double)threshold100Percent );
	existenceProbability *= 0.000'000'1f; // To somewhat keep numbers from completely exploding to 0 or inf

	if (existenceProbability >= 1) // Exit early if probability is 100% or more
	{
		return true;
	}

	// Gets probability as ratio e.g. 1:1000(1 in every thousand) but only this part 1:-->1000<--
	long double probability1OutOfX = 1 / existenceProbability; // Note that existenceProbability is smaller than 1
	bool result = true;

	if (probability1OutOfX <= INT_MAX)
	{
		result = (rand() % (int)probability1OutOfX) == 1; // 1 is just an arbitrary number here
		return result;
	}

	unsigned long long powersOfINT_MAX = (unsigned long long)ceil( logCustomBase(INT_MAX, probability1OutOfX) ); // How often you need to square INT_MAX to get probability1OutOf1X(doubles can get BIG)
	long long rangeLimit = (long long)nthRoot((long long)powersOfINT_MAX, probability1OutOfX); // The range limit so that (INT_MAX % rangeLimt)^powerOfINT_MAX = probability1OutOfX
	
	// Use multiple rand() mapped to the rangeLimit's range to emulate probabilities far lower than 1 / INT_MAX, since the probability of a rare event happening gets squared when asking if it were to happen multiple times
	for (unsigned long long i = 0; i < powersOfINT_MAX; i++)
	{
		int tmp = rand() % rangeLimit;
		if (tmp != 1) result = false;
	}

	return result;
}

bool mapPointExistenceProbabilityExponential32
(
	Vector3_Int32 point,
	long thresholdX,
	long thresholdY,
	long thresholdZ
) 
{
	if (point.x == 0 ||
		point.y == 0 ||
		point.z == 0 ||
		thresholdX == 0 ||
		thresholdY == 0 ||
		thresholdZ == 0 
	)
	{
		perror( formatString("%s No component of point is allowed to be 0 nor is any threshold allowed to be 0! (MathUtils.c, (mapPointExistenceProbabilityExponential32))", ERROR_TAG) );
	}
	bool xExists = mapNumberExistenceProbabilityExponential(point.x, thresholdX);
	bool yExists = mapNumberExistenceProbabilityExponential(point.y, thresholdY);
	bool zExists = mapNumberExistenceProbabilityExponential(point.z, thresholdZ);
	bool pointExists =  xExists && yExists && zExists;
	
	return pointExists;
}

bool mapPointExistenceProbabilityExponential64
(
	Vector3_Int64 point,
	long long thresholdX,
	long long thresholdY,
	long long thresholdZ
) 
{
	bool xExists = mapNumberExistenceProbabilityExponential(point.x, thresholdX);
	bool yExists = mapNumberExistenceProbabilityExponential(point.y, thresholdY);
	bool zExists = mapNumberExistenceProbabilityExponential(point.z, thresholdZ);
	bool pointExists = xExists && yExists && zExists;

	return pointExists;
}

unsigned long getPointsDistanceInt32(Vector3_Int32 point1, Vector3_Int32 point2)
{
	unsigned long distance = 0;
	double dx = point1.x - point2.x;
	double dy = point1.y - point2.y;
	double dz = point1.y - point2.y;
	distance = (unsigned long)sqrt( dx * dx + dy * dy + dz * dz );
	return distance;
}

unsigned long long getPointsDistanceInt64(Vector3_Int64 point1, Vector3_Int64 point2)
{
	unsigned long long distance = 0;
	double dx = (double)(point1.x - point2.x);
	double dy = (double)(point1.y - point2.y);
	double dz = (double)(point1.y - point2.y);
	distance = (unsigned long long)sqrt(dx * dx + dy * dy + dz * dz);

	return distance;
}

// Vector Math

Vector3_Float32 normalizeVector3Float32(Vector3_Float32 vector)
{
	float vectorSumComponentsAbsolute = 0;

	vectorSumComponentsAbsolute = (float)(fabsf(vector.x)
                                        + fabsf(vector.y)
									    + fabsf(vector.z));

	vector.x = vector.x / vectorSumComponentsAbsolute;
	vector.y = vector.y / vectorSumComponentsAbsolute;
	vector.z = vector.z / vectorSumComponentsAbsolute;

	return vector;
}

Vector3_Double64 normalizeVector3Double64(Vector3_Double64 vector)
{
	double vectorLargestComponentAbsolute = 0;

	vectorLargestComponentAbsolute = fabs(vector.x)
                                   + fabs(vector.y)
		                           + fabs(vector.z);

	vector.x = vector.x / vectorLargestComponentAbsolute;
	vector.y = vector.y / vectorLargestComponentAbsolute;
	vector.z = vector.z / vectorLargestComponentAbsolute;

	return vector;
}

Vector3_Int32 allignVectorWithTargetVector32
(
	Vector3_Int32 vector, 
	Vector3_Float32 targetVector // targetVector already as Vector3Float32 so that input can already be normalized
) 
{
	Vector3_Float32 targetVectorDirectionNormalized = {0};
	targetVectorDirectionNormalized = normalizeVector3Float32(targetVector); // Target vector should already be normalized but normalize it anyway just in case

	float vectorMagnitude = 0.0f;
	vectorMagnitude = (float)getPointsDistanceInt32(ORIGIN_POINT_VECTOR3_INT32, vector);
	
	Vector3_Int32 resultVector = {0};
	resultVector.x = (int32_t)(targetVectorDirectionNormalized.x * vectorMagnitude);
	resultVector.y = (int32_t)(targetVectorDirectionNormalized.y * vectorMagnitude);
	resultVector.z = (int32_t)(targetVectorDirectionNormalized.z * vectorMagnitude);

	return resultVector;
}

Vector3_Int64 allignVectorWithTargetVector64
(
	Vector3_Int64 vector,
	Vector3_Double64 targetVector // targetVector already as Vector3Double64 so that input can already be normalized
)
{
	Vector3_Double64 targetVectorDirectionNormalized = { 0 };
	targetVectorDirectionNormalized = normalizeVector3Double64(targetVector); // Target vector should already be normalized but normalize it anyway just in case

	double vectorMagnitude = 0.0;
	vectorMagnitude = (double)getPointsDistanceInt64(ORIGIN_POINT_VECTOR3_INT64, vector);

	Vector3_Int64 resultVector = { 0 };
	resultVector.x = (int64_t)(targetVectorDirectionNormalized.x * vectorMagnitude);
	resultVector.y = (int64_t)(targetVectorDirectionNormalized.y * vectorMagnitude);
	resultVector.z = (int64_t)(targetVectorDirectionNormalized.z * vectorMagnitude);

	return resultVector;
}

Vector3_Float32 createVectorFromTargetDirectionAndMagnitude_Float32(Vector3_Float32 targetDirection, float magnitude)
{
	Vector3_Float32 result = {0};
	targetDirection = normalizeVector3Float32(targetDirection);

	result.x = targetDirection.x * magnitude;
	result.y = targetDirection.y * magnitude;
	result.z = targetDirection.z * magnitude;

	return result;
}

Vector3_Double64 createVectorFromTargetDirectionAndMagnitude_Double64(Vector3_Double64 targetDirection, double magnitude)
{
	Vector3_Double64 result = { 0 };
	targetDirection = normalizeVector3Double64(targetDirection);

	result.x = targetDirection.x * magnitude;
	result.y = targetDirection.y * magnitude;
	result.z = targetDirection.z * magnitude;

	return result;
}

Vector3_Int32 Vector3Int32CrossProduct(Vector3_Int32 vectorA, Vector3_Int32 vectorB)
{
	Vector3_Int32 result = {0};
	
	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Int64 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Int64 crossProduct = Vector3Int64CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3_Int64 Vector3Int64CrossProduct(Vector3_Int64 vectorA, Vector3_Int64 vectorB)
{
	Vector3_Int64 result = { 0 };

	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Float32 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Float32 crossProduct = Vector3Float32CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3_Float32 Vector3Float32CrossProduct(Vector3_Float32 vectorA, Vector3_Float32 vectorB)
{
	Vector3_Float32 result = { 0 };

	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}

Vector3_Double64 Vector3Double64CrossProduct(Vector3_Double64 vectorA, Vector3_Double64 vectorB)
{
	Vector3_Double64 result = { 0 };

	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}
