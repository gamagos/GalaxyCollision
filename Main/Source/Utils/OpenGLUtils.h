// (C) Sebastian Fiault
#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdbool.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "DataUtils.h"
#include "FileUtils.h"

/*
SYNOPSIS:
	Function for glfwSetErrorCallback - to make debugging easier
DESCRIPTION:
	Literally just the function that we will give to OpenGL to use to print
	error messages via callback.
ARGS:
	code:
		The error code
	description:
		The description of the error
*/
void glfw_error_callback(int code, const char* description);

/*
SYNOPSIS:
	Function for glfwSetFramebufferSizeCallback. Makes the window resize correctly on resize events.
ARGS:
	window:
		The OpenGL window in question
	width & height:
		The new width and height of said window
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
SYNOPSIS:
	Callback function to process input on input events
ARGS:
	window:
		The window for which to process input for
*/
void processInput(GLFWwindow* window);

/*
SYNOPSIS:
	Creates and compiles a shader of a specified type from a specified source.
ARGS:
	path:
		The path to the shader source code IN THE BUILD DIR
	pathIsRelativePath:
		Wether "path" a relative path or not(absolute path).
	shaderType:
		Which type of shader the shader should be as a GLenum
RETURNS:
	The id associated by OpenGL with the created shader or NULL if not successful
REMARKS:
	"path" must be in the build dir!
*/
#ifndef infoLogSizeShader
#define infoLogSizeShader 8192U
#endif
GLuint createShaderFromPath(const char* path, bool pathIsRelativePath, GLenum shaderType);

/*
SYNOPSIS:
	Creates a shader program from the supplied shaders and deletes the shaders after usage
ARGS:
	shaders:
		The shaders to use in the shader program
	amountShaders:
		How many shaders are inside of the array "shaders"
RETURNS:
	The id of the newly created shader program or NULL if something goes wrong
REMARKS:
	THIS FUNTION DELETES THE SUPPLIED SHADERS!
*/
#ifndef infoLogSizeShaderProgram
#define infoLogSizeShaderProgram 8192U
#endif
GLuint createShaderProgram(GLuint shaders[], unsigned int amountShaders);

#endif
