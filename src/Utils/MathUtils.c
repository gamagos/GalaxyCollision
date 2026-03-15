// (C) Sebastian Fiault 2026
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "../Constants.h"
#include "../Types.h"

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
	Maps the existence probability of a point to e(-x) for each coordinate
	normalized for some value minX/Y/Z that represent the value that is used for 100% probability.
	And then returns the wether the point exists or not as a bool, the closer the points is to the origin the
	more likely it is to exist.
DESCRIPTION:
	The function does this by mapping each probability of the points coordinate to e(-x) normalized for maxX/Y/Z
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
bool mapPointExistenceProbabilityExponential32(
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
bool mapPointExistenceProbabilityExponential64(
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
