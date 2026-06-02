// Fragment Shader

#version 460 core

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

in vec2 outTextureCoordinates;

out vec4 color;

void main() {
	vec2 smileyTextureCoordinates = vec2(
		outTextureCoordinates.x,
		outTextureCoordinates.y
	);

	color = mix(
		texture(ourTexture1, outTextureCoordinates),
		texture(ourTexture2, smileyTextureCoordinates), 0.4 
	);
}