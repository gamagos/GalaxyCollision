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
uniform float drag;
//uniform float deltaTime;
uniform float forceMinimum;
uniform float mass;
uniform float speedCap;
//uniform float timeWarp;

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
vec3 origin = vec3(0.0f, 0.0f, 0.0f);
float distanceBodies = 0.0f;
float force = 0.0f;

void main()
{
	// Update positions based on velocities
	data[gl_VertexID].position.x += data[gl_VertexID].velocity.x;
	data[gl_VertexID].position.y += data[gl_VertexID].velocity.y;
	data[gl_VertexID].position.z += data[gl_VertexID].velocity.z;

	// ### Do physics ###
	for (uint i = 0; i < amountStars; i ++)
	{
		if (i == gl_VertexID) continue; // Skip if current point is self
		distanceBodies = distance(data[gl_VertexID].position, data[i].position);
		if (distanceBodies > distanceMaximum) continue;
		force = getGravitationalForce(mass, mass, distanceBodies);
		normalizedDirectionVector = data[i].position - data[gl_VertexID].position;
		normalizedDirectionVector = normalize(normalizedDirectionVector);
		data[gl_VertexID].velocity.x += getAcceleration(force * normalizedDirectionVector.x, mass);
		data[gl_VertexID].velocity.y += getAcceleration(force * normalizedDirectionVector.y, mass);
		data[gl_VertexID].velocity.z += getAcceleration(force * normalizedDirectionVector.z, mass);
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

	// Apply drag
	data[gl_VertexID].velocity.x /= drag;
	data[gl_VertexID].velocity.y /= drag;
	data[gl_VertexID].velocity.z /= drag;

	// Apply gravity to origin(0, 0, 0) to keep particles centered and to keep particles returning if they are to far away and the force applied to them is bellow the force minimum
	normalizedDirectionVector = origin - data[gl_VertexID].position;
	normalizedDirectionVector = normalize(normalizedDirectionVector);
	data[gl_VertexID].velocity.x += getAcceleration(forceMinimum * normalizedDirectionVector.x, mass);
	data[gl_VertexID].velocity.y += getAcceleration(forceMinimum * normalizedDirectionVector.y, mass);
	data[gl_VertexID].velocity.z += getAcceleration(forceMinimum * normalizedDirectionVector.z, mass);

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
