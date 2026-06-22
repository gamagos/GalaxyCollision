#ifndef PHYSICS_UTILS_H
#define PHYSICS_UTILS_H

#include <math.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"

/*
SYNOPSIS:
	Literally just a wrapper function for the standard physics equation a = F / m
ARGS:
	force:
		The force that is applied to the object
	mass:
		The mass of the object
RETURNS:
	The resulting acceleration of the object
REMARKS:
	DOES NOT USE UNITS YET!
*/
float getAcceleration_32(float force, float mass);

/*
SYNOPSIS:
	Get the gravitational force that 2 bodies at a given distance generate on each other
ARGS:
	massBody1:
		The mass of the first body
	massBody2:
		The mass of the second body
	distanceOfBodies:
		The distance the 2 bodies have from each other
RETURNS:
	The resulting gravitational force
REMARKS:
	DOES NOT USE UNITS YET!
*/
float getGravitationalForce_32(float massBody1, float massBody2, float distanceOfBodies);

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
float getRequiredOrbitVelocity32(float orbitRadius, float massCenterObject, float massOrbitingObject);

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
double getRequiredOrbitVelocity64(double orbitRadius, double massCenterObject, double massOrbitingObject);

#endif