// (C) Sebastian Fiault 2026
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../Constants.h"
#include "../Types.h"
#include "DataUtils.h"
#include "MathUtils.h"

/*
SYNOPSIS:
	Get the absolute value of a floating point number
ARGS:
	number:
		The number to get the absolute value from
RETURNS:
	The absolute value of the number
EXAMPLE:
double a = -2;
a = myAbs(a); ==> 2
*/
double myAbs(double number)
{
	number = sqrt(number * number);
	return number;
}

/*
SYNOPSIS:
	Logarithm for any base
DESCRIPTION:
	Gets the result of the logarithm for any base using
	the change of base of logarithms rule
ARGS:
	base:
		The base of the logarithm
	_X:
		The actually number to get the logarithm for
		the base for
RETURNS:
	The result of the logarithm with base "base" and _X
EXAMPLE:
	double a = logCustomBase(8, 256);
*/
double logCustomBase(double base, double _X)
{
	return log(_X) / log(base);
}

/*
SYNOPSIS:
	Gets the n-th root of a number
DESCRIPTION:
	Gets the n-th root of a number 
ARGS:
	n:
		Which root to get.
		n = 2 pulls the square root,
		n = 3 pulls the cubic root
		...
	number:
		The number to pull the nth root from
RETURNS:
	n-th root of number
REMARKS:
	Remember negative numbers don't have roots!
*/
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

/*
SYNOPSIS:
	Maps the probability of a number existing to
	the function e^(-x) and then returns true or false with that probability.
ARGS:
	number:
		The number who's existence probability is to be mapped
	threshold100Percent:
		Which number on e^(-x) represents a probability of 100%
RETURNS:
	Either true or false, the lower the probability of number
	existing the more likely it will return false
REMARKS:
	It should be avoided to input numbers smaller than thershold100Percent because
	then their probability of existing is >=100%
EXAMPLE:
	bool doesMyNumberExist = mapNumberExistenceProbabilityExponential(2994, 0);
*/
bool mapNumberExistenceProbabilityExponential(long long number, long long threshold100Percent)
{
	srand((unsigned int)time(NULL));
	// Map the probability of the numbers existence to e^(-x) and normalize it for one x to be 100%
	double existenceProbability = exp( (double)-number ) / exp( (double)-threshold100Percent );
	
	if (existenceProbability >= 1) // Exit early if probability is 100% or more
	{
		return true;
	}

	// Gets probability as ratio e.g. 1:1000(1 in every thousand) but only this part 1:-->1000<--
	double probability1OutOfX = 1 / existenceProbability;  
	bool result = true;

	if (probability1OutOfX <= INT_MAX)
	{
		result = (rand() % (int)probability1OutOfX) == 1;
		return result;
	}

	unsigned long long powersOfINT_MAX = (unsigned long long)ceil( logCustomBase(INT_MAX, probability1OutOfX) ); // How often you need to square INT_MAX to get probability1OutOf1X(doubles can get BIG)
	long long rangeLimit = (long long)nthRoot((long long)powersOfINT_MAX, probability1OutOfX); // The range limit so that (INT_MAX % rangeLimt)^powerOfINT_MAX = probability1OutOfX
	
	// Use multiple rand() mapped to the rangeLimit's range to emulate probabilities far lower than 1 / INT_MAX, since the the probability of a rare event happening gets squared when asking if it were to happen multiple times
	for (unsigned long long i = 0; i < powersOfINT_MAX; i++)
	{
		int tmp = rand() % rangeLimit;
		if (tmp != 1) result = false;
	}

	return result;
}

/*
SYNOPSIS:
	Get's the distance of 2 points as a positive number
DEPENDECIES:
	Vector3Int32 struct from Types.h
ARGS:
	point1, point2:
		The points to get the distance from
RETURNS
	The distance of the 2 points as a positive long int.
REMARKS:
	Looses some precision because math functions use double
EXAMPLE:
	Vector3Int32 pointA = {2,1,1}, pointB = {2,2,1};
	unsigned long distance = getPointsDistanceInt32(pointA, pointB);
	printf("Distance points: %ld", distance); ==> Distance points: 1
*/
unsigned long getPointsDistanceInt32(Vector3Int32 point1, Vector3Int32 point2)
{
	unsigned long distance = 0;
	double dx = point1.x - point2.x;
	double dy = point1.y - point2.y;
	double dz = point1.y - point2.y;
	distance = (unsigned long)sqrt( dx * dx + dy * dy + dz * dz );
	return distance;
}

/*
SYNOPSIS:
	Get's the distance of 2 points as a positive number
DEPENDECIES:
	Vector3Int64 struct from Types.h
ARGS:
	point1, point2:
		The points to get the distance from
RETURNS
	The distance of the 2 points as a positive long int.
REMARKS:
	Looses some precision because math functions use double
EXAMPLE:
	Vector3Int64 pointA = {2,1,1}, pointB = {2,2,1};
	unsigned long distance = getPointsDistance64(pointA, pointB);
	printf("Distance points: %ld", distance); ==> Distance points: 1
*/
unsigned long long getPointsDistanceInt64(Vector3Int64 point1, Vector3Int64 point2)
{
	unsigned long long distance = 0;
	double dx = (double)(point1.x - point2.x);
	double dy = (double)(point1.y - point2.y);
	double dz = (double)(point1.y - point2.y);
	distance = (unsigned long long)sqrt(dx * dx + dy * dy + dz * dz);

	return distance;
}

/*
SYNOPSIS:
	Maps the existence probability of a point to e(-x) for each coordinate
	normalized for some value minX/Y/Z that represent the value that is used for 100% probability.
	And then returns the wether the point exists or not as a bool, the closer the points is to the origin the
	more likely it is to exist.
DESCRIPTION:
	The function does this by mapping each probability of the points coordinate to e(-x) normalized for maxX/Y/Z
	to be 100%, if all points return true on their existence the function returns true.
DEPENDENCIES:
	VectorInt32 struct from Types.h
ARGS:
	point:
		The point to be mapped for
	thresholdX:
		The 100% probability threshold for x
	thresholdY:
		The 100% probability threshold for x
	thresholdZ:
		The 100% probability threshold for x
RETURNS:
	True or false, the further the point is from the origin the more likely it will return false
REMARKS:
	Not 100% precise but pretty precise
EXAMPLE:
	Vector3Int32 point = {35,545,46};
	bool doesMyPointExist = mapPointExistenceProbabilityExponential(point, 0, 0, 0);
*/
bool mapPointExistenceProbabilityExponential32
(
	Vector3Int32 point,
	long thresholdX,
	long thresholdY,
	long thresholdZ
) 
{
	bool xExists = mapNumberExistenceProbabilityExponential(point.x, thresholdX);
	bool yExists = mapNumberExistenceProbabilityExponential(point.y, thresholdY);
	bool zExists = mapNumberExistenceProbabilityExponential(point.z, thresholdZ);
	bool pointExists =  xExists && yExists && zExists;
	
	return pointExists;
}

/*
SYNOPSIS:
	Maps the existence probability of a point to e(-x) for each coordinate
	normalized for some value minX/Y/Z that represent the value that is used for 100% probability.
	And then returns the wether the point exists or not as a bool, the closer the points is to the origin the
	more likely it is to exist.
DESCRIPTION:
	The function does this by mapping each probability of the points coordinate to e(-x) normalized for maxX/Y/Z
	to be 100%, if all points return true on their existence the function returns true.
DEPENDENCIES:
	VectorInt64 struct from Types.h
ARGS:
	point:
		The point to be mapped for
	thresholdX:
		The 100% probability threshold for x
	thresholdY:
		The 100% probability threshold for x
	thresholdZ:
		The 100% probability threshold for x
RETURNS:
	True or false, the further the point is from the origin the more likely it will return false
REMARKS:
	Not 100% precise but pretty precise
EXAMPLE:
	Vector3Int32 point = {35,545,46};
	bool doesMyPointExist = mapPointExistanceProbabilityExponential(point, 0, 0, 0);
*/
bool mapPointExistenceProbabilityExponential64
(
	Vector3Int64 point,
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

// Vector Math

/*
SYNOPSIS:
	Normalizes a vector's components for 1
DESCRIPTION:
	Normalizes a vector's components for 1 by
	getting the ratios for all components to the largest components
	in %
DEPENDENCIES:
	Vector3Float32 from Types.h
ARGS:
	vector:
		The vector to be normalized
RETURNS:
	The normalized vector
EXAMPLE:
	Vector3Float32 myVector = {1.0f, 2.0f, 2.0f};
	myVector = normalizeVector32(myVector); ==> {0.5f, 1.0f, 1.0f}
*/
Vector3Float32 normalizeVector3Float32(Vector3Float32 vector)
{
	float vectorSumComponentsAbsolute = 0;

	vectorSumComponentsAbsolute = (float)(myAbs(vector.x)
                                        + myAbs(vector.y)
									    + myAbs(vector.z));

	vector.x = vector.x / vectorSumComponentsAbsolute;
	vector.y = vector.y / vectorSumComponentsAbsolute;
	vector.z = vector.z / vectorSumComponentsAbsolute;

	return vector;
}

/*
SYNOPSIS:
	Normalizes a vector's components for 1
DESCRIPTION:
	Normalizes a vector's components for 1 by
	getting the ratios for all components to the largest components
	in %
DEPENDENCIES:
	Vector3Double64 from Types.h
ARGS:
	vector:
		The vector to be normalized
RETURNS:
	The normalized vector
EXAMPLE:
	Vector3Double64 myVector = {1.0, 2.0, 0.5};
	myVector = normalizeVector32(myVector); ==> {0.5, 1.0, 0.25}
*/
Vector3Double64 normalizeVector3Double64(Vector3Double64 vector)
{
	double vectorLargestComponentAbsolute = 0;

	vectorLargestComponentAbsolute = myAbs(vector.x)
                                   + myAbs(vector.y)
		                           + myAbs(vector.z);

	vector.x = vector.x / vectorLargestComponentAbsolute;
	vector.y = vector.y / vectorLargestComponentAbsolute;
	vector.z = vector.z / vectorLargestComponentAbsolute;

	return vector;
}

/*
SYNOPSIS:
	Aligns a vector with the direction of a target vector while keeping it's amplitude
DEPENDENCIES:
	Vector3Int32 and Vector3Float32 from Types.h
ARGS:
	vector:
		The vector that is to be aligned
	targetVector:
		The vector to align the vector to. 
		Vector is a Vector3Float32 so that input can already be
		normalized and to aid precision of alignment
RETURNS:
	Vector with the same amplitude as the provided vector with the direction of the target vector
REMARKS:
	Looses precision because of floating point math utilization under the hood
EXAMPLE:
	Vector3Int32 myVector = {1,1,2};
	Vector3Float32 direction = {0.0f, 1.0f, 0.0f};
	myVector = allignVectorWithTargetVector32(myVector, targetVector); ==> {0, sqrt(6), 0}
*/
Vector3Int32 allignVectorWithTargetVector32
(
	Vector3Int32 vector, 
	Vector3Float32 targetVector // targetVector already as Vector3Float32 so that input can already be normalized
) 
{
	Vector3Float32 targetVectorDirectionNormalized = {0};
	targetVectorDirectionNormalized = normalizeVector3Float32(targetVector); // Target vector should already be normalized but normalize it anyway just in case

	float vectorMagnitude = 0.0f;
	vectorMagnitude = (float)getPointsDistanceInt32(ORIGIN_POINT_VECTOR3_INT32, vector);
	
	Vector3Int32 resultVector = {0};
	resultVector.x = (int32_t)(targetVectorDirectionNormalized.x * vectorMagnitude);
	resultVector.y = (int32_t)(targetVectorDirectionNormalized.y * vectorMagnitude);
	resultVector.z = (int32_t)(targetVectorDirectionNormalized.z * vectorMagnitude);

	return resultVector;
}

/*
SYNOPSIS:
	Aligns a vector with the direction of a target vector while keeping it's amplitude
DEPENDENCIES:
	Vector3Int64 and Vector3Double64 from Types.h
ARGS:
	vector:
		The vector that is to be aligned
	targetVector:
		The vector to align the vector to.
		Vector is a Vector3Double64 so that input can already be
		normalized and to aid precision of alignment
RETURNS:
	Vector with the same amplitude as the provided vector with the direction of the target vector
REMARKS:
	Looses precision because of floating point math utilization under the hood
EXAMPLE:
	Vector3Int64 myVector = {1,1,2};
	Vector3Double64 direction = {0.0, 1.0, 0.0};
	myVector = allignVectorWithTargetVector64(myVector, targetVector); ==> {0, sqrt(6), 0}
*/
Vector3Int64 allignVectorWithTargetVector64
(
	Vector3Int64 vector,
	Vector3Double64 targetVector // targetVector already as Vector3Double64 so that input can already be normalized
)
{
	Vector3Double64 targetVectorDirectionNormalized = { 0 };
	targetVectorDirectionNormalized = normalizeVector3Double64(targetVector); // Target vector should already be normalized but normalize it anyway just in case

	double vectorMagnitude = 0.0;
	vectorMagnitude = (double)getPointsDistanceInt64(ORIGIN_POINT_VECTOR3_INT64, vector);

	Vector3Int64 resultVector = { 0 };
	resultVector.x = (int64_t)(targetVectorDirectionNormalized.x * vectorMagnitude);
	resultVector.y = (int64_t)(targetVectorDirectionNormalized.y * vectorMagnitude);
	resultVector.z = (int64_t)(targetVectorDirectionNormalized.z * vectorMagnitude);

	return resultVector;
}

/*
SYNOPSIS:
	Creates a vector with a given magnitude in a given direction
DESCRIPTION:
	Takes a target vector, get's its direction and then creates a new vector
	in th direction of the target vector, with the specified magnitude
DEPENDENCIES:
	Vector3Float32 from Types.h
ARGS:
	targetDirection:
		A vector that points in the same direction the new vector is supposed to point
	magnitude:
		The magnitude/length the new vector is supposed to have
RETURNS:
	A new Vector that points the same direction as targetDirection and is as long as magnitude
EXAMPLE:
	Vecto3Float32 direction = {0.0f, 1.0f, 1.5f};
	float magnitude = 10;
	Vector3Float32 newVector = createVectorFromTargetDirectionAndMagnitude_Float32(direction, magnitude);
*/
Vector3Float32 createVectorFromTargetDirectionAndMagnitude_Float32(Vector3Float32 targetDirection, float magnitude)
{
	Vector3Float32 result = {0};
	targetDirection = normalizeVector3Float32(targetDirection);

	result.x = targetDirection.x * magnitude;
	result.y = targetDirection.y * magnitude;
	result.z = targetDirection.z * magnitude;

	return result;
}

/*
SYNOPSIS:
	Creates a vector with a given magnitude in a given direction
DESCRIPTION:
	Takes a target vector, get's its direction and then creates a new vector
	in th direction of the target vector, with the specified magnitude
DEPENDENCIES:
	Vector3Float32 from Types.h
ARGS:
	targetDirection:
		A vector that points in the same direction the new vector is supposed to point
	magnitude:
		The magnitude/length the new vector is supposed to have
RETURNS:
	A new Vector that points the same direction as targetDirection and is as long as magnitude
EXAMPLE:
	Vecto3Double64 direction = {0.0, 1.0, 1.5};
	double magnitude = 10;
	Vector3Double64 newVector = createVectorFromTargetDirectionAndMagnitude_Float32(direction, magnitude);
*/
Vector3Double64 createVectorFromTargetDirectionAndMagnitude_Double64(Vector3Double64 targetDirection, double magnitude)
{
	Vector3Double64 result = { 0 };
	targetDirection = normalizeVector3Double64(targetDirection);

	result.x = targetDirection.x * magnitude;
	result.y = targetDirection.y * magnitude;
	result.z = targetDirection.z * magnitude;

	return result;
}

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
DEPENDENCIES:
	Vector3Int32 from Types.h
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Int32 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Int32 crossProduct = Vector3Int32CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3Int32 Vector3Int32CrossProduct(Vector3Int32 vectorA, Vector3Int32 vectorB)
{
	Vector3Int32 result = {0};
	
	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
DEPENDENCIES:
	Vector3Int64 from Types.h
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Int64 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Int64 crossProduct = Vector3Int64CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3Int64 Vector3Int64CrossProduct(Vector3Int64 vectorA, Vector3Int64 vectorB)
{
	Vector3Int64 result = { 0 };

	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
DEPENDENCIES:
	Vector3Float32 from Types.h
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Float32 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Float32 crossProduct = Vector3Float32CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3Float32 Vector3Float32CrossProduct(Vector3Float32 vectorA, Vector3Float32 vectorB)
{
	Vector3Float32 result = { 0 };

	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
DEPENDENCIES:
	Vector3Double64 from Types.h
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Double64 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Double64 crossProduct = Vector3Double64CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3Double64 Vector3Double64CrossProduct(Vector3Double64 vectorA, Vector3Double64 vectorB)
{
	Vector3Double64 result = { 0 };

	result.x = (vectorA.y * vectorB.z - vectorA.z * vectorB.y);
	result.y = (vectorA.z * vectorB.x - vectorA.x * vectorB.z);
	result.z = (vectorA.x * vectorB.y - vectorA.y * vectorB.x);

	return result;
}
