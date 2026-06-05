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
#include "Utils/GenerationUtils.h"
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
    int indexPrimaryVBO = 0;
    glVertexAttribPointer(indexPrimaryVBO, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0); // Current buffer layout: | float posX | float posY | float posZ |
    glEnableVertexAttribArray(indexPrimaryVBO);

    char* vertexShaderSourcePath = getAbsolutePath("Main/Resources/shaders/Vertex.glsl"); //! Apparently you need to specify paths as if we were inside the build directory!
    char* vertexShaderSource = readFileAsCharArray(vertexShaderSourcePath);
    GLuint vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
    char* fragmentShaderSourcePath = getAbsolutePath("Main/Resources/shaders/Fragment.glsl");
    char* fragmentShaderSource = readFileAsCharArray(fragmentShaderSourcePath);
    GLuint fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    GLuint shaders[] = {
        vertexShader,
        fragmentShader
    };
    GLuint primaryShaderProgram = createShaderProgram(shaders, 2);
    safer_free(&vertexShaderSourcePath);
    safer_free(&vertexShaderSource);
    safer_free(&fragmentShaderSourcePath);
    safer_free(&fragmentShaderSource);

    printf_s("Using OpenGL version %s\n", glGetString(GL_VERSION));
    // ##############################################################################

    printf_s("Starting galaxy generation\n");
    BlackHole_32 parentBlackHole = {
        .position_Terameters = (Vector3_Int32){0, 0, 0},
        .velocity_KilometersPerSecond = (Vector3_Float32){0.0f, 0.0f, 0.0f},
        .mass_10_BillionQuettagrams = 10'000.0f,
        .standardGravitationalParameter_TerametersCubedPerPetaecondSquared = 0.0f, // Assigned later
        .rotationAxis = (Vector3_Float32){0.0f, 1.0f, 0.0f},
        .rotatesClockwise = true
    };
    parentBlackHole.standardGravitationalParameter_TerametersCubedPerPetaecondSquared = GRAVITATIONAL_CONSTANT_FLOAT * parentBlackHole.mass_10_BillionQuettagrams;
    unsigned int amountStars = 1'000;
    Star_32* galaxy = generateStars32Galaxy(amountStars, parentBlackHole);
    float* positions = calloc((size_t)(amountStars * 3), sizeof(float));
    if (!positions)
    {
        perror("Failed to allocate memory for positions (Main.c, main())");
        glDeleteProgram(primaryShaderProgram);
        glfwTerminate();
        return 1;
    }

    for ( unsigned int i = 0; i < (amountStars * 3); i += 3 )
    {
        size_t galaxyIndex = i / 3;
        positions[i] = galaxy[galaxyIndex].position_Terameters.x;
        positions[i + 1] = galaxy[galaxyIndex].position_Terameters.y;
        positions[i + 2] = galaxy[galaxyIndex].position_Terameters.z;
    }
    
    // Normalize all values in positions for 
    float positionsMaxValueAbsolute = 0.0f;
    for (unsigned int i = 0; i < (amountStars * 3); i ++)
    {
        if ( fabsf(positions[i]) > positionsMaxValueAbsolute )
        {
            positionsMaxValueAbsolute = fabsf(positions[i]);
        }
    }
    for (unsigned int i = 0; i < (amountStars * 3); i ++)
    {
        positions[i] /= positionsMaxValueAbsolute;
    }
    //! This is for debugging
    for (unsigned int i = 0; i < amountStars; i++)
    {
        printf("%f\n", positions[i]);
        printf("%f\n", positions[i + 1]);
        printf("%f\n", positions[i + 2]);
        printf("\n");
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * amountStars * 3, positions, GL_STATIC_DRAW); // Copy the newly generated galaxy into VRAM
    
    // Setup matrices
    mat4 viewMatrix = {0};
    glm_lookat(
        (vec3){ 0.0f, 0.0f, 10.0f },
        (vec3){ 0.0f, 0.0f, 0.0f },
        (vec3){ 0.0f, 1.0f, 0.0f },
        viewMatrix
    );
    GLuint viewMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix);

    mat4 perspectiveProjectionMatrix = {0};
    glm_mat4_identity(perspectiveProjectionMatrix);
    glm_perspective( glm_rad(70.0f), (float)windowWidth / (float)windowHeight, 0.1f, 1'000.0f, perspectiveProjectionMatrix );
    GLuint perspectiveProjectionMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(perspectiveProjectionMatrixUniformLocation, 1, GL_FALSE, perspectiveProjectionMatrix);

    printf("Starting render-loop\n");

    // Render loop
    while (!glfwWindowShouldClose(primaryWindow))
    {
        glBindBuffer(GL_ARRAY_BUFFER, primaryVBO);

        glDrawArrays(GL_POINTS, 0, amountStars);

        glfwSwapBuffers(primaryWindow);
        glfwPollEvents();
    }

    glDeleteProgram(primaryShaderProgram);
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