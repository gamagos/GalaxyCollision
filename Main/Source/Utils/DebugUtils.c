// (C) Sebastian Fiault 2026
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "DataUtils.h"

// Quit the program as if an error happened
int quitProgramOnError(
    void* pointersToFree[],
    size_t amountPointers,
    GLuint buffersToDelete[],
    size_t amountBuffers,
    GLuint vertexArraysToDelete[],
    size_t amountVertexArrays,
    GLuint shaderProgramsToDelete[],
    size_t amountShaderPrograms,
    const char* extraInfo
)
{
    const char* stringToPrint = "Something went wrong\n";

    if (extraInfo) 
    {
        perror(formatString("%s %s; %s\n",ERROR_TAG, stringToPrint, extraInfo));
    }
    else
    {
        perror(stringToPrint);
    }

    if (pointersToFree && amountPointers)
    {
        for (size_t i = 0; i < amountPointers; i++)
        {
            if (pointersToFree[i]) safer_free(&pointersToFree[i]);
        }
    }
    else if (pointersToFree)
    {
        perror( formatString("%s You specified \"pointersToFree\" but forgot to specify the amount \"amountPointers\" (DebugUtils.c quitProgram())\n", ERROR_TAG) );
    }

    if (buffersToDelete && amountBuffers)
    {
        glDeleteBuffers(amountBuffers, buffersToDelete);
    }
    else if (buffersToDelete)
    {
        perror( formatString("%s You specified \"buffersToDelete\" but forgot to specify the amount \"amountBuffers\" (DebugUtils.c quitProgram())\n", ERROR_TAG));
    }

    if (vertexArraysToDelete && amountVertexArrays)
    {
        glDeleteVertexArrays(amountVertexArrays, vertexArraysToDelete);
    }
    else if (vertexArraysToDelete)
    {
        perror(formatString("%s You specified \"vertexArraysToDelete\" but forgot to specify the amount \"amountVertexArrays\" (DebugUtils.c quitProgram())\n", ERROR_TAG));
    }

    if (shaderProgramsToDelete && amountShaderPrograms)
    {
        for (size_t i = 0; i < amountShaderPrograms; i++)
        {
            glDeleteProgram(shaderProgramsToDelete[i]);
        }
    }
    else if (shaderProgramsToDelete)
    {
        perror(formatString("%s You specified \"shaderProgramsToDelete\" but forgot to specify the amount \"amountShaderPrograms\" (DebugUtils.c quitProgram())\n", ERROR_TAG));
    }

    glfwTerminate();
    exit(EXIT_FAILURE);
    return 1; // technically redundant
}
