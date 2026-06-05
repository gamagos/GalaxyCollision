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
#include "Utils/PhysicsUtils.h"

// THIS PROGRAM ONLY WORKS FOR WINDOWS

int windowWidth = 1'900;
int windowHeight = 1'000;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(3 * sizeof(float)) );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 10, (void*)(6 * sizeof(float)) );
    glEnableVertexAttribArray(2);

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
    glUseProgram(primaryShaderProgram);
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
    unsigned int amountStars = 1'200;
    Star_32* galaxy = generateStars32Galaxy(amountStars, parentBlackHole);
    float* positions = calloc( (size_t)(amountStars * 3), sizeof(float) );
    if (!positions)
    {
        perror("Failed to allocate memory for positions (Main.c, main())");
        glDeleteProgram(primaryShaderProgram);
        glfwTerminate();
        return 1;
    }
    float* velocities = calloc( (size_t)(amountStars * 3), sizeof(float) );
    if (!velocities)
    {
        perror("Failed to allocate memory for velocities (Main.c, main())");
        glDeleteProgram(primaryShaderProgram);
        glfwTerminate();
        return 1;
    }
    float* colors = calloc( (size_t)(amountStars * 4), sizeof(float) );
    if (!colors)
    {
        perror("Failed to allocate memory for colors (Main.c, main())");
        glDeleteProgram(primaryShaderProgram);
        glfwTerminate();
        return 1;
    }

    for ( unsigned int i = 0; i < (amountStars * 3); i += 3 )
    {
        size_t galaxyIndex = i / 3;
        positions[i]     = galaxy[galaxyIndex].position_Terameters.x;
        positions[i + 1] = galaxy[galaxyIndex].position_Terameters.y;
        positions[i + 2] = galaxy[galaxyIndex].position_Terameters.z;
    }
    for (unsigned int i = 0; i < (amountStars * 3); i += 3)
    {
        size_t galaxyIndex = i / 3;
        velocities[i]     = galaxy[galaxyIndex].velocity_KilometersPerSecond.x;
        velocities[i + 1] = galaxy[galaxyIndex].velocity_KilometersPerSecond.y;
        velocities[i + 2] = galaxy[galaxyIndex].velocity_KilometersPerSecond.z;
    }
    for (unsigned int i = 0; i < (amountStars * 4); i += 4)
    {
        size_t galaxyIndex = i / 4;
        colors[i]     = (float)(galaxy[galaxyIndex].color.Red)   / 255.0f;
        colors[i + 1] = (float)(galaxy[galaxyIndex].color.Green) / 255.0f;
        colors[i + 2] = (float)(galaxy[galaxyIndex].color.Blue)  / 255.0f;
        colors[i + 3] = (float)(galaxy[galaxyIndex].color.Alpha) / 255.0f;
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

    typedef struct dataForBuffer { //! This also is more or less a temporary workaround
        float position[3];
        float velocity[3];
        float color[4];
    } dataForBuffer;

    dataForBuffer* bufferData = calloc( (size_t)amountStars, sizeof(dataForBuffer) );
    if (!bufferData)
    {
        perror("Failed to allocate memory for bufferData (Main.c, main())");
        glDeleteProgram(primaryShaderProgram);
        glfwTerminate();
        return 1;
    }
    size_t i3 = 0;
    size_t i4 = 0;
    for (size_t i = 0; i < amountStars; i++)
    {
        memcpy_s( // Positions 
            &bufferData[i].position, 
            sizeof(float) * 3,
            &positions[i3],
            sizeof(float) * 3
        );
        memcpy_s( // Velocities
            &bufferData[i].velocity,
            sizeof(float) * 3,
            &velocities[i3],
            sizeof(float) * 3
        );
        memcpy_s( // Colors
            &bufferData[i].color,
            sizeof(float) * 4,
            &colors[i4],
            sizeof(float) * 4
        );
        i3 += 3;
        i4 += 4;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(dataForBuffer) * amountStars, bufferData, GL_STATIC_DRAW); // Copy the newly generated galaxy into VRAM
    
    // Setup matrices
    mat4 viewMatrix = {0};
    glm_lookat(
        (vec3){ 0.1f, 0.0f, 4.0f },
        (vec3){ 0.0f, 0.0f, 0.0f },
        (vec3){ 0.0f, 1.0f, 0.0f },
        viewMatrix
    );
    GLuint viewMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix);

    mat4 perspectiveProjectionMatrix = {0};
    glm_perspective( glm_rad(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 1'000'000'000.0f, perspectiveProjectionMatrix );
    GLuint perspectiveProjectionMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(perspectiveProjectionMatrixUniformLocation, 1, GL_FALSE, perspectiveProjectionMatrix);

    glEnable(GL_DEPTH_TEST);
    printf("Starting render-loop\n");

    // ### Variable definitions for loop ###
    // Gravity Physics, so far only movement
    size_t bufferDataIndex = 0;
    float mass = 36'000.0f; // All bodies weight the same for now
    float speedCap = 0.8f;
    float distance = 0.0f;
    float force = 0.0f;
    float drag = 1.001;
    vec3 normalizedDirectionVector = { 0.0f }; //! This vector must remain normalized [1.0f; -1.0f]!
    // Variables for camera
    float camX = 0.0f;
    float camZ = 0.0f;
    float radius = 4.0f;
    float camRotationSpeedReductionDivisor = 5.5f; // bigger means slower

    // Render loop
    while (!glfwWindowShouldClose(primaryWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindBuffer(GL_ARRAY_BUFFER, primaryVBO);
        
        // Move camera
        camX = sin(glfwGetTime() / camRotationSpeedReductionDivisor) * radius;
        camZ = cos(glfwGetTime() / camRotationSpeedReductionDivisor) * radius;
        mat4 viewMatrix = { 0 };
        glm_lookat(
            (vec3) { camX, 0.0f, camZ },
            (vec3) { 0.0f, 0.0f, 0.0f },
            (vec3) { 0.0f, 1.0f, 0.0f },
            viewMatrix
        );
        glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, viewMatrix);

        for (size_t i = 0; i < (size_t)(amountStars * 3); i += 3)
        {
            bufferDataIndex = i / 3;
            positions[i]     += velocities[i];
            positions[i + 1] += velocities[i + 1];
            positions[i + 2] += velocities[i + 2];
            memcpy_s( // Positions 
                &bufferData[bufferDataIndex].position,
                sizeof(float) * 3,
                &positions[i],
                sizeof(float) * 3
            );

            for (size_t j = 0; j < (size_t)(amountStars * 3); j += 3)
            {
                if (i == j) continue; // Skip if point is itself
                // Apply gravity
                distance = glm_vec3_distance(&positions[i], &positions[j]);
                glm_vec3_sub(&positions[j], &positions[i], normalizedDirectionVector);
                glm_vec3_normalize(normalizedDirectionVector);
                force = getGravitationalForce_32(mass, mass, distance);
                velocities[i]     += getAcceleration_32(force * normalizedDirectionVector[0], mass);
                velocities[i + 1] += getAcceleration_32(force * normalizedDirectionVector[1], mass);
                velocities[i + 2] += getAcceleration_32(force * normalizedDirectionVector[2], mass);
            } //! The only thing that needs to be n^2 is gravity!
            // Cap velocity
            velocities[i] = velocities[i] > 0 ? min(velocities[i], speedCap) : max(velocities[i], -speedCap);
            velocities[i + 1] = velocities[i + 1] > 0 ? min(velocities[i + 1], speedCap) : max(velocities[i + 1], -speedCap);
            velocities[i + 2] = velocities[i + 2] > 0 ? min(velocities[i + 2], speedCap) : max(velocities[i + 2], -speedCap);
            // Apply drag
            velocities[i] /= drag;
            velocities[i + 1] /= drag;
            velocities[i + 2] /= drag;

            memcpy_s( // Velocities
                &bufferData[bufferDataIndex].velocity,
                sizeof(float) * 3,
                &velocities[i],
                sizeof(float) * 3
            );
        } //? Currently working on making physics calculations on CPU
        glBufferData(GL_ARRAY_BUFFER, sizeof(dataForBuffer)* amountStars, bufferData, GL_STATIC_DRAW);

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
//TODO use SSBOs to make physics calculations on the GPU

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