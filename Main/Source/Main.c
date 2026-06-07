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
#include <time.h>
#include <Windows.h>

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GalaxyCollision.h"
#include "Types.h"
#include "Utils/DebugUtils.h"
#include "Utils/FileUtils.h"
#include "Utils/GenerationUtils.h"
#include "Utils/OpenGLUtils.h"
#include "Utils/PhysicsUtils.h"
#include "Utils/UserInterActionUtils.h"

// THIS PROGRAM ONLY WORKS FOR WINDOWS

typedef struct timespec timespec;

int windowWidth = 1'000;
int windowHeight = 1'000;
long unsigned int amountStars = 12'900;

float timeSinceStart_PetaSeconds_Float = 0.0f;
double timeSinceStart_PetaSeconds_Double = 0.0;

int main(int argc, char **argv) 
{
    printf_s("Starting program...\n");
    printf_s("Compiled with C Version: %ld\n", __STDC_VERSION__);
    #if defined(__clang__)
        printf_s("Compiled with: Clang %s\n", __clang_version__);
    #elif defined(__ICC) || defined(__INTEL_COMPILER)
        printf_s("Compiled with: The Intel C/C++ Compiler Version %d\n", __INTEL_COMPILER);
    #elif defined(__GNUC__)
        printf_s("Compiled with: GCC Version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #elif defined(_MSC_VER)
        printf_s("Compiler: MSVC Version %d\n", _MSC_VER);
    #else
        printf_s("Compiled with: Unknown compiler\n");
    #endif
    printf_s("System default integer size is %lu bytes (%lu bits)\n", sizeof(int), sizeof(int) * 8);
    
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
        quitProgramOnError((void**)NULL, (size_t)NULL, (GLuint*)NULL, (size_t)NULL, (GLuint*)NULL, (size_t)NULL, (GLuint*)NULL, (size_t)NULL, "Failed to create glfw window");
        return 1;
    }
    glfwMakeContextCurrent(primaryWindow);

    // Load glad
    int successGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (!successGlad)
    {
        perror("Failed to load glad\n");
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
    
    GLuint primaryVBO; //! Only holds color, because physics are on SSBO!
    glGenBuffers(1, &primaryVBO);
    glBindBuffer(GL_ARRAY_BUFFER, primaryVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    GLuint SSBO;
    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    GLuint vertexShader = createShaderFromPath("Main/Resources/shaders/Vertex.glsl", true, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShaderFromPath("Main/Resources/shaders/Fragment.glsl", true, GL_FRAGMENT_SHADER);
    
    GLuint shaders[] = {
        vertexShader,
        fragmentShader
    };
    GLuint primaryShaderProgram = createShaderProgram(shaders, 2);
    glUseProgram(primaryShaderProgram);

    printf_s("Using OpenGL version %s\n", glGetString(GL_VERSION));
    // ##############################################################################

    // Galaxy Generation
    printf_s("Starting galaxy generation\n");

    // Black hole(s)
    BlackHole_32 parentBlackHole = {
        .position_Terameters = (Vector3_Int32){0, 0, 0},
        .velocity_KilometersPerSecond = (Vector3_Float32){0.0f, 0.0f, 0.0f},
        .mass_10_BillionQuettagrams = 10'000.0f,
        .standardGravitationalParameter_TerametersCubedPerPetaecondSquared = 0.0f, // Assigned later
        .rotationAxis = (Vector3_Float32){0.0f, 1.0f, 0.0f},
        .rotatesClockwise = true
    };
    parentBlackHole.standardGravitationalParameter_TerametersCubedPerPetaecondSquared = GRAVITATIONAL_CONSTANT_FLOAT * parentBlackHole.mass_10_BillionQuettagrams;

    Star_32* galaxy = generateStars32Galaxy(amountStars, parentBlackHole);

    // Memory allocations
    float* positions = calloc( (size_t)(amountStars * 4), sizeof(float) );
    if (!positions)
    {
        quitProgramOnError((void*){galaxy}, 1, (GLuint[]) { primaryVBO, SSBO }, 2, (GLuint[]) { VAO1 }, 1, (GLuint[]) { primaryShaderProgram }, 1, "Failed to allocate memory for positions (Main.c, main())");
        return 1; //TODO make buffer that has all pointers that should be freed on quit
        //TODO make the whole quitProgramOnError functionality be implement work and improve the code quality
    }

    float* velocities = calloc( (size_t)(amountStars * 4), sizeof(float) );
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

    // Copying data from galaxy to custom buffers and doing some normalization on the data
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
    safer_free((void**)&galaxy);

    //// Cap positions at range [1;-1]
    //float positionsMaxValueAbsolute = 0.0f;
    //for (unsigned int i = 0; i < (amountStars * 3); i ++)
    //{
    //    if ( fabsf(positions[i]) > positionsMaxValueAbsolute )
    //    {
    //        positionsMaxValueAbsolute = fabsf(positions[i]);
    //    }
    //}
    //for (unsigned int i = 0; i < (amountStars * 3); i ++)
    //{
    //    positions[i] /= positionsMaxValueAbsolute;
    //}

    // Set the average velocity to 0 so that particles do not go to far from the center
    vec3 averageVelocity = { 0.0f };
    for (size_t i = 0; i < (size_t)(amountStars * 3); i += 3) // Get sum of velocities on axis
    {
        averageVelocity[0] += velocities[i];
        averageVelocity[1] += velocities[i + 1];
        averageVelocity[2] += velocities[i + 2];
    }
    averageVelocity[0] /= amountStars;
    averageVelocity[1] /= amountStars;
    averageVelocity[2] /= amountStars;
    for (size_t i = 0; i < (size_t)(amountStars * 3); i += 3) // Apply the opposite of the average velocity of all particles
    {                                                         // to all particles to set their average velocity to 0 so that they do not drift away from the center
        velocities[i]     -= averageVelocity[0];
        velocities[i + 1] -= averageVelocity[1];
        velocities[i + 2] -= averageVelocity[2];
    }

    typedef struct dataForBuffer { //! This also is more or less a temporary workaround
        float position[3];
        float padding;
        float velocity[3];
        float padding2;
        //float color[4];
    } dataForBuffer;

    dataForBuffer* bufferData = calloc( (size_t)amountStars, sizeof(dataForBuffer) );
    if (!bufferData)
    {
        perror("Failed to allocate memory for bufferData (Main.c, main())\n");
        glDeleteProgram(primaryShaderProgram);
        glfwTerminate();
        return 1;
    }

    size_t i4 = 0;
    for (size_t i = 0; i < amountStars; i++)
    {
        memcpy_s( // Positions 
            &bufferData[i].position, 
            sizeof(float) * 4,
            &positions[i4],
            sizeof(float) * 4
        );
        memcpy_s( // Velocities
            &bufferData[i].velocity,
            sizeof(float) * 4,
            &velocities[i4],
            sizeof(float) * 4
        );
        i4 += 4;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * amountStars, colors, GL_STATIC_DRAW); // Copy the newly generated galaxy into VRAM
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vec3) * 2 * amountStars, bufferData, GL_DYNAMIC_DRAW);
    safer_free((void**)& positions);
    safer_free((void**)& velocities);
    safer_free((void**)& colors);
    safer_free((void**)& bufferData);

    glEnable(GL_DEPTH_TEST);
    printKeybinds();
    // TODO This:
    printf_s("%sChanging camera orbiting speed currently cause glitching before speed change\n", WARNING_TAG);
    printf_s("%sChanging camera orbiting direction currently must be done with a very quick tap of the key or else it will glitch out\n", WARNING_TAG);

    // ### Variable definitions for loop ###
    // Gravity Physics, so far only movement
    double timeLastFrame = glfwGetTime();
    double frameRate = 0.0;
    size_t bufferDataIndex = 0;
    float distance = 0.0f;
    float force = 0.0f;
    float mass = 620'000.0f; // All bodies weight the same for now
    float speedCap = 991.0f;
    float drag = 1.002;
    vec3 normalizedDirectionVector = { 0.0f }; //! This vector must remain normalized [1.0f; -1.0f]!
    // Variables for camera
    float camX = 0.0f;
    float camY = 0.0f;
    float camZ = 0.0f;
    float radius = 4.0f;
    float cameraUserInputSpeed = 0.5f;
    float camOrbitingSpeedReductionDivisor = 6.5f; // bigger means slower
    // Other variables
    double secondsToWaitForInfoOutputUpdate = 0.5;
    double secondWaitedForInfoOutPutUpdate = 0.0;

    // ### Setup Uniforms ###
    // Uniforms for graphics
    mat4 viewMatrix = {0};
    glm_lookat(
        (vec3){ 0.1f, 0.0f, 9.0f },
        (vec3){ 0.0f, 0.0f, 0.0f },
        (vec3){ 0.0f, 1.0f, 0.0f },
        viewMatrix
    );
    GLuint viewMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, (GLfloat*)viewMatrix);

    mat4 perspectiveProjectionMatrix = {0};
    glm_perspective( glm_rad(70.0f), (float)windowWidth / (float)windowHeight, 0.1f, 6'000'000.0f, perspectiveProjectionMatrix );
    GLuint perspectiveProjectionMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(perspectiveProjectionMatrixUniformLocation, 1, GL_FALSE, (GLfloat*)perspectiveProjectionMatrix);

    // Uniforms for physics
    GLuint massUniformLocation = glGetUniformLocation(primaryShaderProgram, "mass");
    glUniform1f(massUniformLocation, mass);
    
    GLuint speedCapUniformLocation = glGetUniformLocation(primaryShaderProgram, "speedCap");
    glUniform1f(speedCapUniformLocation, speedCap);
    
    GLuint dragUniformLocation = glGetUniformLocation(primaryShaderProgram, "drag");
    glUniform1f(dragUniformLocation, drag);

    // Uniforms for both
    GLuint amountStarsUniformLocation = glGetUniformLocation(primaryShaderProgram, "amountStars");
    glUniform1ui(amountStarsUniformLocation, amountStars);

    // Make sure everything is bound technically unnecessary
    glBindBuffer(GL_ARRAY_BUFFER, primaryVBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glUseProgram(primaryShaderProgram);
    
    printf_s("Starting render-loop\n\n");

    // ### Render loop ###
    while (!glfwWindowShouldClose(primaryWindow))
    {
        // Set up for next frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // ### Process User Input ###
        // Camera radius
        if (glfwGetKey(primaryWindow, GLFW_KEY_W)) radius = max( radius - cameraUserInputSpeed, 0.0f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_S)) radius += cameraUserInputSpeed;

        // Camera Y height
        if (glfwGetKey(primaryWindow, GLFW_KEY_SPACE)) camY += cameraUserInputSpeed;
        if (glfwGetKey(primaryWindow, GLFW_KEY_RIGHT_CONTROL)) camY -= cameraUserInputSpeed;
        if (glfwGetKey(primaryWindow, GLFW_KEY_LEFT_CONTROL)) camY -= cameraUserInputSpeed;

        // Camera speed
        if (glfwGetKey(primaryWindow, GLFW_KEY_Q)) cameraUserInputSpeed = min( FLT_MAX, cameraUserInputSpeed * 1.05f );
        if (glfwGetKey(primaryWindow, GLFW_KEY_E)) cameraUserInputSpeed = max( FLT_MIN, cameraUserInputSpeed / 1.05f );

        // Camera orbiting speed and direction
        if (glfwGetKey(primaryWindow, GLFW_KEY_A)) camOrbitingSpeedReductionDivisor = min(FLT_MAX, camOrbitingSpeedReductionDivisor * 1.1f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_D)) camOrbitingSpeedReductionDivisor = max(FLT_MIN, camOrbitingSpeedReductionDivisor / 1.1f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_R)) camOrbitingSpeedReductionDivisor *= -1.0f;

        // Particle mass
        if (glfwGetKey(primaryWindow, GLFW_KEY_X)) mass = min(FLT_MAX, mass * 1.003f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_C)) mass = max(FLT_MIN, mass / 1.003f);

        // Drag
        if (glfwGetKey(primaryWindow, GLFW_KEY_F)) drag = min(FLT_MAX, drag * 1.000'002f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_G)) drag = max(1.0f, drag / 1.000'002f);

        // Velocity cap
        if (glfwGetKey(primaryWindow, GLFW_KEY_1)) speedCap = min(FLT_MAX, speedCap * 1.002f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_2)) speedCap = max(FLT_MIN, speedCap / 1.002f);

        // Move camera
        camX = sin(glfwGetTime() / camOrbitingSpeedReductionDivisor) * radius;
        camZ = cos(glfwGetTime() / camOrbitingSpeedReductionDivisor) * radius;
        mat4 viewMatrix = { 0 };
        glm_lookat(
            (vec3) { camX, camY, camZ },
            (vec3) { 0.0f, 0.0f, 0.0f },
            (vec3) { 0.0f, 1.0f, 0.0f },
            viewMatrix
        );
        // Send updated uniforms to GPU
        glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, (GLfloat*)viewMatrix);
        glUniform1f(massUniformLocation, mass);
        glUniform1f(speedCapUniformLocation, speedCap);
        glUniform1f(dragUniformLocation, drag);

        glDrawArrays(GL_POINTS, 0, amountStars); // Draw call

        glfwSwapBuffers(primaryWindow);
        glfwPollEvents();

        // Output
        if (secondsToWaitForInfoOutputUpdate <= secondWaitedForInfoOutPutUpdate) // Limit the amount of times output get's printed
        {
            frameRate = 1 / (glfwGetTime() - timeLastFrame);
            printf_s("\rFPS: %-4.0lf  Particle mass: %-7.2g  Drag: %-10f  Speed cap: %-12.1f  Camera speed: %-8.1f", frameRate, mass, drag, speedCap, cameraUserInputSpeed);
            secondWaitedForInfoOutPutUpdate = 0;
        }
        else
        {
            secondWaitedForInfoOutPutUpdate += glfwGetTime() - timeLastFrame;
        }
        timeLastFrame = glfwGetTime();
    }

    // Exit program
    GLuint buffersToDelete[] = {
        primaryVBO,
        SSBO
    };
    glDeleteBuffers(2, buffersToDelete);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteProgram(primaryShaderProgram);
    glfwTerminate();
    printf_s("\nProgram finished\n================================================================"); // This looks clean af
    return 0;
}

//TODO make functions actually use units
//TODO implement deltatime and timewarp
//TODO make generation be able to generate different shapes
//TODO make methods for calloc and malloc to make that auto check for allocation success to make code more condensed!
//TODO make config for initial values of simulation be read in via json for better usability
//TODO add more camera controls maybe with mouse? But keep orbiting mode!

// Stuff to do at school
//TODO make buffers that keep track of all used pointers, VBOs, VAOs and shaderProgram to make code safer and quitProgramOnError usage easier
//TODO finish implementing quitProgramOnError in functions <-------- priority
//TODO finish general code cleanup
//TODO in cmake make sure target_sources also get ALL non std header files

/*
GOALS
    - Make galaxy simulation
    - Make it multi-threaded
    - Make it render on and compute on GPU
    - Make it use vulkan
    - Make it optimized
    - Make it really pretty
    - Make it cluster computing - Maybe not might be inefficient because of avg network bandwidth
    - Make super clear docs so anyone can read code
    - Make YouTube video?
*/

//* Space is so cool