// (C) Sebastian Fiault
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../Constants.h"
#include "../Types.h"

#ifndef ORIGIN_POINT_VECTOR3_INT32
#define ORIGIN_POINT_VECTOR3_INT32 (Vector3_Int32){0,0,0}
#endif

#ifndef ORIGIN_POINT_VECTOR3_INT64
#define ORIGIN_POINT_VECTOR3_INT64 (Vector3_Int64){0,0,0}
#endif

#ifndef ORIGIN_POINT_VECTOR3_FLOAT32
#define ORIGIN_POINT_VECTOR3_FLOAT32 (Vector3_Float32){0.0f, 0.0f, 0.0f}
#endif

#ifndef ORIGIN_POINT_VECTOR3_DOUBLE64
#define ORIGIN_POINT_VECTOR3_DOUBLE64 (Vector3_Double64){0.0, 0.0, 0.0}
#endif


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
double logCustomBase(double base, double _X);

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
double nthRoot(long long n, double number);

/*
SYNOPSIS:
	Get's the distance of 2 points as a positive number
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
unsigned long getPointsDistanceInt32(Vector3_Int32 point1, Vector3_Int32 point2);

/*
SYNOPSIS:
	Get's the distance of 2 points as a positive number
ARGS:
	point1, point2:
		The points to get the distance from
RETURNS
	The distance of the 2 points as a positive long int.
REMARKS:
	Looses some precision because math functions use double
EXAMPLE:
	Vector3Int64 pointA = {2,1,1}, pointB = {2,2,1};
	unsigned long distance = getPointsDistance32(pointA, pointB);
	printf("Distance points: %ld", distance); ==> Distance points: 1
*/
unsigned long long getPointsDistanceInt64(Vector3_Int64 point1, Vector3_Int64 point2);

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
	bool doesMyNumberExist = mapNumberExistanceProbabilityExponential(2994, 0);
*/
bool mapNumberExistenceProbabilityExponential(unsigned long long number, unsigned long long threshold100Percent);

/*
SYNOPSIS:
	Maps the existence probability of a point to e(-x) for each coordinate
	normalized for some value thresholdX/Y/Z that represent the value that is used for 100% probability.
	And then returns the wether the point exists or not as a bool, the closer the points is to the origin the
	more likely it is to exist.
DESCRIPTION:
	The function does this by mapping each probability of the points coordinate to e(-x) normalized for thresholdX/Y/Z
	to be 100%, if all points return true on their existence the function returns true.
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
	Vector3_Int32 point,
	long thresholdX,
	long thresholdY,
	long thresholdZ
);

/*
SYNOPSIS:
	Maps the existence probability of a point to e(-x) for each coordinate
	normalized for some value thresholdX/Y/Z that represent the value that is used for 100% probability.
	And then returns the wether the point exists or not as a bool, the closer the points is to the origin the
	more likely it is to exist.
DESCRIPTION:
	The function does this by mapping each probability of the points coordinate to e(-x) normalized for thresholdX/Y/Z
	to be 100%, if all points return true on their existence the function returns true.
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
	Vector3_Int64 point,
	long long thresholdX,
	long long thresholdY,
	long long thresholdZ
);

/*
SYNOPSIS:
	Normalizes a vector's components for 1
DESCRIPTION:
	Normalizes a vector's components for 1 by
	getting the ratios for all components to the largest components
	in %
ARGS:
	vector:
		The vector to be normalized
RETURNS:
	The normalized vector
EXAMPLE:
	Vector3Float32 myVector = {1.0f, 2.0f, 2.0f};
	myVector = normalizeVector32(myVector); ==> {0.5f, 1.0f, 1.0f}
*/
Vector3_Float32 normalizeVector3Float32(Vector3_Float32 vector);

/*
SYNOPSIS:
	Normalizes a vector's components for 1
DESCRIPTION:
	Normalizes a vector's components for 1 by
	getting the ratios for all components to the largest components
	in %
ARGS:
	vector:
		The vector to be normalized
RETURNS:
	The normalized vector
EXAMPLE:
	Vector3Double64 myVector = {1.0, 2.0, 0.5};
	myVector = normalizeVector32(myVector); ==> {0.5, 1.0, 0.25}
*/
Vector3_Double64 normalizeVector3Double64(Vector3_Double64 vector);

/*
SYNOPSIS:
	Aligns a vector with the direction of a target vector while keeping it's amplitude
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
Vector3_Int32 allignVectorWithTargetVector32
(
	Vector3_Int32 vector,
	Vector3_Float32 targetVector // targetVector already as Vector3Float32 so that input can already be normalized
);

/*
SYNOPSIS:
	Aligns a vector with the direction of a target vector while keeping it's amplitude
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
Vector3_Int64 allignVectorWithTargetVector64
(
	Vector3_Int64 vector,
	Vector3_Double64 targetVector // targetVector already as Vector3Double64 so that input can already be normalized
);

/*
SYNOPSIS:
	Creates a vector with a given magnitude in a given direction
DESCRIPTION:
	Takes a target vector, get's its direction and then creates a new vector
	in th direction of the target vector, with the specified magnitude
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
Vector3_Float32 createVectorFromTargetDirectionAndMagnitude_Float32(Vector3_Float32 targetDirection, float magnitude);

/*
SYNOPSIS:
	Creates a vector with a given magnitude in a given direction
DESCRIPTION:
	Takes a target vector, get's its direction and then creates a new vector
	in th direction of the target vector, with the specified magnitude
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
Vector3_Double64 createVectorFromTargetDirectionAndMagnitude_Double64(Vector3_Double64 targetDirection, double magnitude);

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
Vector3_Float32 Vector3Float32CrossProduct(Vector3_Float32 vectorA, Vector3_Float32 vectorB);

/*
SYNOPSIS:
	Calculates the cross product of 2 Vector3s in a 3d coordinate system
ARGS:
	vectorA, vectorB:
		The 2 vectors to get the cross product for
RETURNS:
	The cross product of vectorA and vectorB
EXAMPLE:
	Vector3Double64 a = {1, 2, 3,}, b = {3, 2, 1};
	Vector3Double64 crossProduct = Vector3Double64CrossProduct(a, b); ==> {-4, 8, -4}
*/
Vector3_Double64 Vector3Double64CrossProduct(Vector3_Double64 vectorA, Vector3_Double64 vectorB);

#endif
