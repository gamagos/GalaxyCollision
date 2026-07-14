// (C) Sebastian Fiault

// Vertex Shader

#version 460 core

#ifndef	GRAVITATIONAL_CONSTANT_FLOAT
#define GRAVITATIONAL_CONSTANT_FLOAT 6.67430e-11f
#endif

struct Data 
{
	vec3 position;
	float _padding; // Padding bytes are unused
	vec3 velocity;
	float _padding2;
};

layout(std430, binding = 0) buffer bufferData
{
	Data data[];
};

// Only value that can stay in a the VBO
layout(location = 0) in vec4 color;

// Uniforms for graphics
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// Uniforms for physics
uniform float distanceMaximum;
uniform float deltaTime_Seconds;
uniform float FPSminimum;
uniform float mass;
uniform float speedCap;
uniform float timeWarp;

// Uniforms for both
uniform uint amountStars;

// Variables to pass to fragment shader
out vec4 _color;

float getGravitationalForce(float massBody1, float massBody2, float distanceBodies) // F = G * (m1 * m2) / (d^2)
{
	float force = 0.0f;
	force = GRAVITATIONAL_CONSTANT_FLOAT * (
		(massBody1 * massBody2) / (distanceBodies * distanceBodies)
	);

	return force;
}

float getAcceleration(float force, float massBody) // a = F / m
{
	float acceleration = 0.0f;
	acceleration = force / massBody;

	return acceleration;
}

// Variables for main
vec3 accelerationToOrigin = vec3(0.0f, 0.0f, 0.0f);
vec3 averageVelocity = vec3(0.0f, 0.0f, 0.0f);
vec3 normalizedDirectionVector = vec3(0.0f, 0.0f, 0.0f);
float distanceBodies = 0.0f;
float force = 0.0f;

void main()
{
	if (deltaTime_Seconds != 0 && deltaTime_Seconds / timeWarp < 1.0f / FPSminimum) // This is to prevent major physics glitches. If delta time is 0 all velocities effectively get reset and if delta time is too big a major loss of accuracy occurs. This prevents both the physics from breaking and transient frame drops from ruining physical accuracy
	{
		// Update positions based on velocities
		data[gl_VertexID].position.x += (data[gl_VertexID].velocity.x * deltaTime_Seconds); //TODO fix deltaTime_Seconds / actually implement deltaTime_Seconds
		data[gl_VertexID].position.y += (data[gl_VertexID].velocity.y * deltaTime_Seconds);
		data[gl_VertexID].position.z += (data[gl_VertexID].velocity.z * deltaTime_Seconds);

		// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		//
		// Do physics
		//
		// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
		for (uint i = 0; i < amountStars; i ++)
		{
			if (i == gl_VertexID) continue; // Skip if current point is self
			distanceBodies = distance(data[gl_VertexID].position, data[i].position);
			if (distanceBodies > distanceMaximum) continue;
			force = getGravitationalForce(mass, mass, distanceBodies);
			normalizedDirectionVector = data[i].position - data[gl_VertexID].position;
			normalizedDirectionVector = normalize(normalizedDirectionVector);
			data[gl_VertexID].velocity.x += ( getAcceleration(force * normalizedDirectionVector.x, mass) * deltaTime_Seconds ); //TODO maybe make this use GLSL native support for vectors and matrices so that this can become a one liner
			data[gl_VertexID].velocity.y += ( getAcceleration(force * normalizedDirectionVector.y, mass) * deltaTime_Seconds );
			data[gl_VertexID].velocity.z += ( getAcceleration(force * normalizedDirectionVector.z, mass) * deltaTime_Seconds );
		}

		// Cap velocity
		data[gl_VertexID].velocity.x = data[gl_VertexID].velocity.x > 0 ?
			min(data[gl_VertexID].velocity.x, speedCap) :
			max(data[gl_VertexID].velocity.x, -speedCap);
		data[gl_VertexID].velocity.y = data[gl_VertexID].velocity.y > 0 ?
			min(data[gl_VertexID].velocity.y, speedCap) :
			max(data[gl_VertexID].velocity.y, -speedCap);
		data[gl_VertexID].velocity.z = data[gl_VertexID].velocity.z > 0 ?
			min(data[gl_VertexID].velocity.z, speedCap) :
			max(data[gl_VertexID].velocity.z, -speedCap);
	}

	// Set variables that get passed down pipeline
	gl_Position = projectionMatrix * viewMatrix * vec4(
		data[gl_VertexID].position.x,
		data[gl_VertexID].position.y,
		data[gl_VertexID].position.z,
		1.0f
	);
	_color = color;
}
//TODO try out if doing operations on every single coordinate is really necessary or if you can just do vec * vec
//TODO find out why there is always an immobile point at the center; there indeed appears one point at the center that is immobile no matter what, do not why yet
