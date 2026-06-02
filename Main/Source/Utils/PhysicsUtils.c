#include <math.h>

#include "../Constants.h"

/*
SYNOPSIS:
	Gets the needed velocity to orbit a given Object with a given Mass
ARGS:
	orbitRadius:
		The radius of the orbit(crazy right),
		in Terameters (= 10^12 meters)
	massCenterObject:
		The mass of the object that the object in question is orbiting around,
		in 10^6 Quettagrams (= 10^6 * 10^30 grams)
RETURNS:
	The velocity at which the object needs to move to have a circular orbit around the center orbit
EXAMPLE:
	float starMass = 1'000.0f;
	float orbitRadius = 100'000.0f;
	float massBlackHole = 10'000.0f;
	float orbitVelocity = getRequiredOrbitVelocity(orbitRadius, massBlackHole, starMass);
*/
float getRequiredOrbitVelocity32(float orbitRadius, float massCenterObject, float massOrbitingObject)
{
	double velocity;
	velocity = sqrt((GRAVITATIONAL_CONSTANT_DOUBLE * (massOrbitingObject + massCenterObject)) / orbitRadius);
	velocity *= pow(10.0, 24.0);

	return (float)velocity;
}
//? left off on making the velocity formulas use V = √((GM + m) R) for more accurate results and implementing "" <> in the Dependency docs
/*
SYNOPSIS:
	Gets the needed velocity to orbit a given Object with a given Mass
ARGS:
	orbitRadius:
		The radius of the orbit(crazy right),
		in Kilometers (= 10^3 meters)
	massCenterObject:
		The mass of the object that the object in question is orbiting around,
		in 10^6 Quettagrams (= 10^6 * 10^30 grams)
	massOrbitingObject:
		The mass of the object that is orbiting the center object
		and of which we want to get the velocity
		in 10^6 Quettagrams (= 10^6 * 10^30 grams)
RETURNS:
	The velocity at which the object needs to move to have a circular orbit around the center orbit
EXAMPLE:
	double starMass = 1'000.0;
	double blackHoleMass = 1'000'000.0;
	double orbitRadius = 1'000.0;
	double orbitVelocity = getRequiredOrbitVelocity(orbitRadius, blackHoleMass, starMass);
*/
double getRequiredOrbitVelocity64(double orbitRadius, double massCenterObject, double massOrbitingObject)
{
	double velocity;
	velocity = sqrt((GRAVITATIONAL_CONSTANT_DOUBLE * (massCenterObject + massOrbitingObject)) / orbitRadius);
	velocity *= pow(10.0, 33.0);

	return velocity;
}
