// (C) Sebastian Fiault
#include <stdio.h>
#include <stdbool.h>

// Third party libraries
#include "../../Libraries/cglm-0.9.6/include/cglm/cglm.h"
#include "../../Libraries/glad/include/glad/glad.h"
#include "../../Libraries/glfw-3.4/include/GLFW/glfw3.h"

#include "../../Include/Constants.h"
#include "../../Include/GalaxyCollision.h"
#include "../../Include/Types.h"
#include "../../Include/Utils/DataUtils.h"
#include "../../Include/Utils/FileUtils.h"

void glfw_error_callback(int code, const char* description)
{
	perror( formatString("\nGLFW error %d: %s", code, description) );
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height); // TODO make it also adjust the projection matrix to not be stretched res
	parametersFor_cglm_perspective* parametersCglmPerspective = glfwGetWindowUserPointer(window);
	glm_perspective(
		parametersCglmPerspective->fovy,
		(float)width / (float)height,
		parametersCglmPerspective->nearZ,
		parametersCglmPerspective->farZ,
		*(parametersCglmPerspective->dest)
	);
	parametersCglmPerspective->matrixGotChanged = true;
}

#ifndef infoLogSizeShader
#define infoLogSizeShader 8192U
#endif
GLuint createShaderFromPath(char* path, bool pathIsRelativePath, GLenum shaderType) //! Relative path must be relative to the build dir, not source dir!
{
	printf_s("Compiling shader from file %s\n", path);
	// Check for invalid input
	if (!path) 
	{
		perror("Function createShaderFromPath()'s \"path\" can not be NULL\n");
		return (GLuint)(uintptr_t)NULL;
	}

	char* shaderSourcePath = 0;
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
		perror( formatString("\n%s Compilation was not successful of shader with source:\n%s\nfrom %s\n", WARNING_TAG, shaderSource, shaderSourcePath));
		glGetShaderInfoLog(shader, infoLogSizeShader, NULL, infoLog);
		perror( formatString("\nShader compilation log:\n%s\n", infoLog) );
		return (GLuint)(uintptr_t)NULL;
	}
	printf("Compilation of shader was successful\n");

	safer_free( (void**)&shaderSourcePath );
	safer_free( (void**)&shaderSource );
	return shader;
}

#ifndef infoLogSizeShaderProgram
#define infoLogSizeShaderProgram 8192U
#endif
GLuint createShaderProgram(GLuint shaders[], unsigned int amountShaders)
{
	printf("Linking shaders\n");
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
		return (GLuint)(uintptr_t)NULL;
	}
	printf("Linking shaders was successful\n");

	for (unsigned int i = 0; i < amountShaders; i++) {
		glDeleteShader(shaders[i]);
	}

	return shaderProgram;
}
