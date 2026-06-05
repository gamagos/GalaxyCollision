#include <math.h>

#include "../Constants.h"

float getAcceleration_32(float force, float mass)
{
	float acceleration = 0.0f;
	acceleration = force / mass;
	return acceleration;
}

float getGravitationalForce_32(float massBody1, float massBody2, float distanceOfBodies)
{
	float force = 0.0f;
	force = GRAVITATIONAL_CONSTANT_FLOAT * ( (massBody1 * massBody2) / (distanceOfBodies * distanceOfBodies) );
	return force;
}

float getRequiredOrbitVelocity32(float orbitRadius, float massCenterObject, float massOrbitingObject)
{
	double velocity;
	velocity = sqrt((GRAVITATIONAL_CONSTANT_DOUBLE * (massOrbitingObject + massCenterObject)) / orbitRadius);
	velocity *= pow(10.0, 24.0);

	return (float)velocity;
}
//? left off on making the velocity formulas use V = √((GM + m) R) for more accurate results

double getRequiredOrbitVelocity64(double orbitRadius, double massCenterObject, double massOrbitingObject)
{
	double velocity;
	velocity = sqrt((GRAVITATIONAL_CONSTANT_DOUBLE * (massCenterObject + massOrbitingObject)) / orbitRadius);
	velocity *= pow(10.0, 33.0);

	return velocity;
}
