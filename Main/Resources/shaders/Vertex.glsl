// Vertex Shader

#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec4 myColor;

out vec4 _color;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position.x, position.y, position.z, 1.0f);
	_color = myColor;
}
