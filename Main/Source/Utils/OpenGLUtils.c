// (C) Sebastian Fiault
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

#include "../Constants.h"
#include "DataUtils.h"
#include "FileUtils.h"

void glfw_error_callback(int code, const char* description)
{
	perror( formatString("\nGLFW error %d: %s", code, description) );
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

#ifndef infoLogSizeShader
#define infoLogSizeShader 8192U
#endif
GLuint createShaderFromPath(const char* path, bool pathIsRelativePath, GLenum shaderType) //! Relative path must be relative to the build dir, not source dir!
{
	// Check for invalid input
	if (!path) 
	{
		perror("Function createShaderFromPath()'s \"path\" can not be NULL");
		return NULL;
	}

	const char* shaderSourcePath = 0;
	if (pathIsRelativePath)
	{
		shaderSourcePath = getAbsolutePath(path);
	}
	else // If path already is an absolute path
	{
		shaderSourcePath = path;
	}
	const char* shaderSource = readFileAsCharArray(shaderSourcePath);

	GLuint shader = 0;
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	
	// Check for compilation success
	int success = 0;
	char infoLog[infoLogSizeShader] = {0};
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{	
		perror( formatString("\n%s Shader compilation not successful of shader with source:\n%s", WARNING_TAG, shaderSource) );
		glGetShaderInfoLog(shader, infoLogSizeShader, NULL, infoLog);
		perror( formatString("\nShader compilation log:\n%s", infoLog) );
		return NULL;
	}

	safer_free(&shaderSourcePath);
	safer_free(&shaderSource);
	return shader;
}

#ifndef infoLogSizeShaderProgram
#define infoLogSizeShaderProgram 8192U
#endif
GLuint createShaderProgram(GLuint shaders[], unsigned int amountShaders)
{
	GLuint shaderProgram = 0;
	int success = 0;
	char infoLog[infoLogSizeShaderProgram] = {0};

	shaderProgram = glCreateProgram();

	for (unsigned int i = 0; i < amountShaders; i++)
	{
		glAttachShader(shaderProgram, shaders[i]);
	}
	
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		perror( formatString("\n%s Linking shaders was not successful", WARNING_TAG) );
		glGetProgramInfoLog(shaderProgram, infoLogSizeShaderProgram, NULL, infoLog);
		perror( formatString("\nShader linking log:\n%s", infoLog));
		return NULL;
	}

	for (unsigned int i = 0; i < amountShaders; i++) {
		glDeleteShader(shaders[i]);
	}

	return shaderProgram;
}
