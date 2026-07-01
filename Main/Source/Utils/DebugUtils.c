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
const char* getBuildPlatform() // Get current architecture, detects nearly every architecture. Coded by Freak
{ 
    #if defined(__x86_64__) || defined(_M_X64)
        return "x86_64";
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
        return "x86_32";
    #elif defined(__ARM_ARCH_2__)
        return "ARM2";
    #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
        return "ARM3";
    #elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
        return "ARM4T";
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
        return "ARM5"
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
        return "ARM6T2";
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
        return "ARM6";
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7";
    #elif defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7A";
    #elif defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
        return "ARM7R";
    #elif defined(__ARM_ARCH_7M__)
        return "ARM7M";
    #elif defined(__ARM_ARCH_7S__)
        return "ARM7S";
    #elif defined(__aarch64__) || defined(_M_ARM64)
        return "ARM64";
    #elif defined(mips) || defined(__mips__) || defined(__mips)
        return "MIPS";
    #elif defined(__sh__)
        return "SUPERH";
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
        return "POWERPC";
    #elif defined(__PPC64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
        return "POWERPC64";
    #elif defined(__sparc__) || defined(__sparc)
        return "SPARC";
    #elif defined(__m68k__)
        return "M68K";
    #else
        return "UNKNOWN";
    #endif
}

