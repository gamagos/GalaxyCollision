// Vertex Shader

#version 460 core

//uniform float offsetY;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTextureCoordinates;

out vec2 outTextureCoordinates;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position.x, position.y/* + offsetY*/, position.z, 1.0f);
	outTextureCoordinates = inTextureCoordinates;
}
