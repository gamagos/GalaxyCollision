#include <math.h>

#include "../Constants.h"

/*
SYNOPSIS:
	Gets the needed velocity to orbit a given Object with a given Mass
ARGS:
	orbitRadius:
		The radius of the orbit(crazy right)
	massCenterObject:
		The mass of the object that the object in question is orbiting around
RETURNS:
	The velocity at which the object needs to move to have a circular orbit around the center orbit
EXAMPLE:
	double starMass = 1000;
	double orbitRadius = 1'000'000'000
	double orbitVelocity = getRequiredOrbitVelocity();
*/
double getRequiredOrbitVelocity(double orbitRadius, double massCenterObject)
{
	double velocity;
	velocity = sqrt( (massCenterObject * GRAVITATIONAL_CONSTANT_DOUBLE) / orbitRadius );

	return velocity;
}
