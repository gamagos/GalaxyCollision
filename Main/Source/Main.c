// (C) Sebastian Fiault 2026

/*
This is not a scientific program!
This program is for visual purposes.
This program uses metric units for the representation of data instead of
astrological units.
SI prefixes are applied to make data fit into 32 bit floating point variables.

PROJECT DEPENDECIES:
       C23 or newer
       cglm
       glad
       gflw3
       WINDOWS LIBS:
           windows.h
*/

#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Types.h"
#include "Utils/FileUtils.h"
#include "Utils/OpenGLUtils.h"

// THIS PROGRAM ONLY WORKS FOR WINDOWS

int windowWidth = 800;
int windowHeight = 800;

float timeSinceStart_PetaSeconds_Float = 0.0f;
double timeSinceStart_PetaSeconds_Double = 0.0;

int main(int argc, char **argv) 
{
    printf_s("Starting program...\n");
    printf_s("Compiled with C Version: %ld\n", __STDC_VERSION__);
    printf_s("System default integer size is %llu bytes (%llu bits)\n", sizeof(int), sizeof(int) * 8);
    
    // ##############################################################################
    // Initialize OpenGL, glad and glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(glfw_error_callback);

    // Create primary window
    GLFWwindow* primaryWindow = glfwCreateWindow(windowWidth, windowHeight, "Galaxy Collision", NULL, NULL);
    if (!primaryWindow)
    {
        perror("Failed to create GLFW window.\nExiting...\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(primaryWindow);
    // Load glad
    int successGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!successGlad)
    {
        perror("Failed to load glad\nExiting...\n");
        glfwTerminate();
        return 1;
    }
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(primaryWindow, framebuffer_size_callback);

    // Setup/Config
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color

    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);

    GLuint primaryVBO;
    glGenBuffers(1, &primaryVBO);
    glBindBuffer(GL_ARRAY_BUFFER, primaryVBO);
    //TODO glBufferData();
    //TODO glVertexAttribPointer();
    //TODO glEnableVertexAttribArray();

    char* vertexShaderSourcePath = getAbsolutePath("Main/Resources/shaders/Vertex.shader"); //! Apparently you need to specify paths as if we were inside the build directory!
    char* vertexShaderSource = readFileAsCharArray(vertexShaderSourcePath);
    GLuint vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    char* fragmentShaderSourcePath = getAbsolutePath("Main/Resources/shaders/Fragment.shader");
    char* fragmentShaderSource = readFileAsCharArray(fragmentShaderSourcePath);
    GLuint fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    GLuint shaders[] = {
        vertexShader,
        fragmentShader
    };
    GLuint shaderProgram = createShaderProgram(shaders, 2);

    printf_s("Using OpenGL version %s\n", glGetString(GL_VERSION));
    // ##############################################################################

    printf_s("Starting galaxy generation\n");
    glfwTerminate();
    printf_s("Program finished\n==========================================================");
    return 0;
}

//TODO add GPLv3 License
//TODO make functions actually use units!

/*
GOALS
    - Make galaxy simulation
    - Make it multi-threaded
    - Make it render on and compute on GPU
    - Make it use vulkan
    - Make it optimized
    - Make it really pretty
    - Make it cluster computing
    - Make super clear docs so anyone can read code
    - Make YouTube video?
*/

//* Space is so cool