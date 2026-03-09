// (C) Sebastian Fiault 2026
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../Constants.h"


bool mapStareExistenceProbabilityExponential(Vector3 starPosition)
{

}

double mapValueExponentialDouble64(double factor, double value, double maxValue)
{
	return (double)( (factor * exp(value)) % maxValue);
}