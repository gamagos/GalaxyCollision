// (C) Sebastian Fiault 2026
#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Constants.h"
#include "../GalaxyCollision.h"
#include "DataUtils.h"


/*
SYNOPSIS:
    Safely quits the program if called
ARGS:
    pointersToFree:
        All pointers that still need to be freed, NULL if there are no pointers that need freeing
    amountPointers:
        How many pointers are in pointersToFree, NULL if there are no pointers that need freeing
    buffersToDelete:
        All OpenGL buffer objects that were made using glGenBuffers() that need to be deleted, NULL if there are no buffers that need to be deleted
    amountBuffers:
        How many buffers there are in buffersToDelete, NULL if there are no buffers
    vertexArraysToDelete:
        All OpenGL VAOs that were made using glGenVertexArrays() that need to be deleted, NULL if there are none that need to be deleted
    amountVertexArrays:
        How many vertex arrays there are in vertexArraysToDelete, NULL if there are none
    shaderProgramsToDelete:
        All shader programs that need to be delete, NULL if there are none
    amountShaderPrograms
RETURNS:
    1
REMARKS:
    All pointers that get freed also get assigned 0 to prevent dangling pointer issues.
    Generally you can always use NULL if you have no objects to pass to one of this methods parameters.
    This function automatically adds newlines behind the string extrainfo!
*/
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
);

// Source - https://stackoverflow.com/a/66249936
// Posted by FreakAnon, modified by community. See post 'Timeline' for change history
// Retrieved 2026-06-23, License - CC BY-SA 4.0
// Slightly modified by gamagos a.k.a. Sebastian Fiault
/*
    SYNOPSIS:
        Get a string with architecture that the project was built for
    DESCRIPTION:
        Uses macros to determine the architecture that this project was built for to select different return statements,
        which contain strings with the different architecture names
    RETURNS:
        A char array with the name of the architecture the project was built for
*/
const char* getBuildPlatform(); // Get current architecture, detects nearly every architecture. Coded by Freak

#endif