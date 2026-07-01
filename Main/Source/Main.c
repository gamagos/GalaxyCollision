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

// For quitProgramOnError
void* pointersCurrentlyInUse[1024]; //! Not for special pointers, only meant for pointers to memory on heap for malloc, calloc or realloc
size_t currentIndexPointersCurrentlyInUse = 0;
GLuint bufferObjectsCurrentlyInUse[1024];
size_t currentIndexbufferObjectsCurrentlyInUse = 0;
GLuint vertexArraysCurrentlyInUse[1024];
size_t currentIndexvertexArraysCurrentlyInUse = 0;

int windowWidth = 1'000;
int windowHeight = 1'000;
long unsigned int amountStars = 4'000;

float timeSinceStart_PetaSeconds_Float = 0.0f;
double timeSinceStart_PetaSeconds_Double = 0.0;

int main(int argc, char **argv) 
{
    printf_s("Starting Program\n");
    printf_s("Compiled with C Version: %ld\n", __STDC_VERSION__);
    #if defined(__clang__) //TODO move this macro to a separate method
        printf_s("Compiled with: Clang %s\n", __clang_version__);
    #elif defined(__ICC) || defined(__INTEL_COMPILER)
        printf_s("Compiled with: The Intel C/C++ Compiler Version %d\n", __INTEL_COMPILER);
    #elif defined(__GNUC__)
        printf_s("Compiled with: GCC Version: %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #elif defined(_MSC_VER)
        printf_s("Compiled with: MSVC Version %d\n", _MSC_VER);
    #else
        printf_s("Compiled with: Unknown compiler\n");
    #endif
    const char* projectTargetPlatform = getBuildPlatform();
    printf_s("Compiled for Platform: %s\n", projectTargetPlatform);
    printf_s("This Project Was Built for Microsoft Windows\n"); //TODO make this adapt for different OSs
    printf_s("System default integer size is %llu bytes (%llu bits)\n", sizeof(int), sizeof(int) * 8);
    printf_s("System default pointer size is %llu bits\n", sizeof(size_t) * 8);
    
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
    printf_s("Using OpenGL version %s\n", glGetString(GL_VERSION));
    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetFramebufferSizeCallback(primaryWindow, framebuffer_size_callback);

    // Setup/Config
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color

    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    vertexArraysCurrentlyInUse[currentIndexvertexArraysCurrentlyInUse++] = VAO1;
    
    GLuint primaryVBO; //! Only holds color, because physics are on SSBO!
    glGenBuffers(1, &primaryVBO);
    glBindBuffer(GL_ARRAY_BUFFER, primaryVBO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    bufferObjectsCurrentlyInUse[currentIndexbufferObjectsCurrentlyInUse++] = primaryVBO;

    GLuint SSBO;
    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
    bufferObjectsCurrentlyInUse[currentIndexbufferObjectsCurrentlyInUse++] = SSBO;

    GLuint vertexShader = createShaderFromPath("Main/Resources/shaders/Vertex.glsl", true, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShaderFromPath("Main/Resources/shaders/Fragment.glsl", true, GL_FRAGMENT_SHADER);
    
    GLuint shaders[] = {
        vertexShader,
        fragmentShader
    };
    GLuint primaryShaderProgram = createShaderProgram(shaders, 2);
    glUseProgram(primaryShaderProgram);
    // ##############################################################################

    printf_s("Particle count for current simulation: %lu\n", amountStars);

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
    pointersCurrentlyInUse[currentIndexPointersCurrentlyInUse++] = galaxy;

    // Memory allocations
    float* positions = calloc( (size_t)(amountStars * 4), sizeof(float) ); 
    pointersCurrentlyInUse[currentIndexPointersCurrentlyInUse++] = positions;
    if (!positions)
    {
        quitProgramOnError(pointersCurrentlyInUse, currentIndexPointersCurrentlyInUse + 1,
                           bufferObjectsCurrentlyInUse, currentIndexbufferObjectsCurrentlyInUse + 1,
                           vertexArraysCurrentlyInUse, currentIndexvertexArraysCurrentlyInUse,
                           (GLuint[]) {primaryShaderProgram}, 1,
                           "Failed to allocate memory for positions (Main.c, main())");
        return 1; //TODO make buffer that has all pointers that should be freed on quit
        //TODO make the whole quitProgramOnError functionality be implement work and improve the code quality
    }

    float* velocities = calloc( (size_t)(amountStars * 4), sizeof(float) );
	pointersCurrentlyInUse[currentIndexPointersCurrentlyInUse++] = velocities;
    if (!velocities)
    {
        quitProgramOnError(pointersCurrentlyInUse, currentIndexPointersCurrentlyInUse + 1,
                           bufferObjectsCurrentlyInUse, currentIndexbufferObjectsCurrentlyInUse + 1,
                           vertexArraysCurrentlyInUse, currentIndexvertexArraysCurrentlyInUse,
                           (GLuint[]) {primaryShaderProgram}, 1,           
                           "Failed to allocate memory for velocities (Main.c, main())");
        return 1;
    }

    float* colors = calloc( (size_t)(amountStars * 4), sizeof(float) );
    pointersCurrentlyInUse[currentIndexPointersCurrentlyInUse++] = colors;
    if (!colors)
    {
        quitProgramOnError(pointersCurrentlyInUse, currentIndexPointersCurrentlyInUse + 1,
                           bufferObjectsCurrentlyInUse, currentIndexbufferObjectsCurrentlyInUse + 1,
                           vertexArraysCurrentlyInUse, currentIndexvertexArraysCurrentlyInUse,
                           (GLuint[]) {primaryShaderProgram}, 1,
                           "Failed to allocate memory for colors (Main.c, main())");
        return 1;
    }

    // Copying data from galaxy to custom buffers and doing some normalization on the data
    size_t incrementForForLoop = 4;
    for (size_t i = 0; i < (amountStars * 4); i += incrementForForLoop )
    {
        size_t galaxyIndex = i / incrementForForLoop;
		positions[i]     = galaxy[galaxyIndex].position_Terameters.x; //DELETME: This does not work for some random reason
        positions[i + 1] = galaxy[galaxyIndex].position_Terameters.y;
        positions[i + 2] = galaxy[galaxyIndex].position_Terameters.z;
        positions[i + 3] = 0; // for padding bytes
    }

    //! Temporary
    //TODO make a method for this
    // Separate the positions into two separate blocks so that those can collide and look cool
    float offsetXposition = 4.0f;
    float offsetYposition = 2.0f;
    float offsetZposition = 0.0f;
    for (size_t i = 0; i < ( (amountStars * 4) / 2 ); i += incrementForForLoop) // first half
    {
        positions[i] += offsetXposition;
        positions[i + 1] += offsetYposition;
        positions[i + 2] += offsetZposition;
    }
    for (size_t i = ((amountStars * 4) / 2); i < (amountStars * 4); i += incrementForForLoop) // second half
    {
        positions[i] -= offsetXposition;
        positions[i + 1] -= offsetYposition;
        positions[i + 2] -= offsetZposition;
    }

    for (size_t i = 0; i < (amountStars * 4); i += incrementForForLoop)
    {
        size_t galaxyIndex = i / incrementForForLoop;
        velocities[i]     = galaxy[galaxyIndex].velocity_KilometersPerSecond.x;
        velocities[i + 1] = galaxy[galaxyIndex].velocity_KilometersPerSecond.y;
        velocities[i + 2] = galaxy[galaxyIndex].velocity_KilometersPerSecond.z;
        velocities[i + 3] = 0; // for padding bytes
    }
    
    for (size_t i = 0; i < (amountStars * 4); i += 4)
    {
        size_t galaxyIndex = i / 4;
        colors[i]     = (float)(galaxy[galaxyIndex].color.Red)   / 255.0f;
        colors[i + 1] = (float)(galaxy[galaxyIndex].color.Green) / 255.0f;
        colors[i + 2] = (float)(galaxy[galaxyIndex].color.Blue)  / 255.0f;
        colors[i + 3] = (float)(galaxy[galaxyIndex].color.Alpha) / 255.0f;
    }
    safer_free((void**)&galaxy);

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
    //! Temporary
    //TODO make a method for this
    // Give the 2 clusters velocities that make them collide
    float offsetXvelocity = -0.29f;
    float offsetYvelocity = 0.0f;
    float offsetZvelocity = 0.0f;
    for (size_t i = 0; i < ((amountStars * 4) / 2); i += incrementForForLoop) // first half
    {
        velocities[i] += offsetXvelocity;
        velocities[i + 1] += offsetYvelocity;
        velocities[i + 2] += offsetZvelocity;
    }
    for (size_t i = ((amountStars * 4) / 2); i < (amountStars * 4); i += incrementForForLoop) // second half
    {
        velocities[i] -= offsetXvelocity;
        velocities[i + 1] -= offsetYvelocity;
        velocities[i + 2] -= offsetZvelocity;
    }

    typedef struct dataForBuffer { //! This also is more or less a temporary workaround
        float position[3];
        float padding;
        float velocity[3];
        float padding2;
    } dataForBuffer;

    dataForBuffer* bufferData = calloc( (size_t)amountStars, sizeof(dataForBuffer) );
    pointersCurrentlyInUse[currentIndexPointersCurrentlyInUse++] = bufferData;
    if (!bufferData)
    {
        quitProgramOnError(pointersCurrentlyInUse, currentIndexPointersCurrentlyInUse + 1,
                           bufferObjectsCurrentlyInUse, currentIndexbufferObjectsCurrentlyInUse + 1,
                           vertexArraysCurrentlyInUse, currentIndexvertexArraysCurrentlyInUse,
                           (GLuint[]) {primaryShaderProgram}, 1,
                           "Failed to allocate memory for bufferData (Main.c, main())");
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

    // ### Variable definitions for loop ###
    // Variables for physics
    double frameRate = 0.0;
    float accelerationMinimum = 0.000'002f;
    float distanceMaximum = 0.0f;
    float drag = 1.002;
	float deltaTime = 0.0f;
    float force = 0.0f;
    float forceMinimum = 0.0f;
    float mass = 1'920'000.0f; // All bodies weight the same for now
    float speedCap = 9'091.0f; //TODO make this be a cap on acceleration instead and make calculate what acceleration to use with distance, preferably the radius of the largest star
	float timeWarp = 1.0f;
    float timeWarpedDeltaTime = 0.0f; // For later use, do not set outside of render loop
    size_t bufferDataIndex = 0;
    vec3 normalizedDirectionVector = { 0.0f }; //! This vector must remain normalized [1.0f; -1.0f]!
    // Variables for camera
    float camX = 0.0f;
    float camY = 0.0f;
    float camZ = 0.0f;
    float camCurrentOrbitAngle = 0.0f;
    float fovY = 70.0f;
    float radius = 4.0f;
    float cameraUserInputSpeed = 0.5f;
    float camOrbitingSpeed = 0.01f; // bigger means slower
    // Other variables
    double secondsToWaitForInfoOutputUpdate = 0.5;
    double secondWaitedForInfoOutPutUpdate = 0.0;
    double timeLastFrame = glfwGetTime();

    // ### Setup Uniforms ###
    // Uniforms for graphics
    mat4 viewMatrix = {0};
    camX = sin(camCurrentOrbitAngle) * radius;
    camZ = cos(camCurrentOrbitAngle) * radius;
    glm_lookat(
        (vec3){ camX, camY, camZ },
        (vec3){ 0.0f, 0.0f, 0.0f },
        (vec3){ 0.0f, 1.0f, 0.0f },
        viewMatrix
    );
    GLuint viewMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, (GLfloat*)viewMatrix);

    mat4 perspectiveProjectionMatrix = {0};
    float aspectRatio = (float)windowWidth / (float)windowHeight;
    float nearZ = 0.1f;
    float farZ = 6'000'000.0f;
    glm_perspective( glm_rad(fovY), aspectRatio, nearZ, farZ, perspectiveProjectionMatrix );
    GLuint perspectiveProjectionMatrixUniformLocation = glGetUniformLocation(primaryShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(perspectiveProjectionMatrixUniformLocation, 1, GL_FALSE, (float*)perspectiveProjectionMatrix);
    // Pass the initial matrix as the initial value to the resize callback function
    parametersFor_cglm_perspective parametersForCglmPerspectiveTemporary = {
        .fovy = fovY,
        .nearZ = nearZ,
        .farZ = farZ,
        .dest = &perspectiveProjectionMatrix,
		.matrixGotChanged = false
    };
    glfwSetWindowUserPointer(primaryWindow, &parametersForCglmPerspectiveTemporary);
	parametersFor_cglm_perspective* parametersForCglmPerspective = 0; // Initialize the variable for later use

    // Uniforms for physics
    GLuint distanceMaximumUniformLocation = glGetUniformLocation(primaryShaderProgram, "distanceMaximum"); // TODO abstract physics into a method
    distanceMaximum = sqrtf( (GRAVITATIONAL_CONSTANT_FLOAT * mass) / accelerationMinimum ); // a = F / m;  F = G * (m1 * m2) / (d^2);  a = (G * m^2 / d^2) / m  <=>  a = G * m / d^2;  Solve for d:  <=>  a * d^2 = G * m  <=>  d^2 = (G * m) / a 
    glUniform1f(distanceMaximumUniformLocation, distanceMaximum);                           // <=>  d = sqrt( (G * m) / a )    
                                                                                            //Note: m1 = m2 here since all objects have the same mass, therefore m1 * m2 = m1^2 or m^2 for short
    GLuint dragUniformLocation = glGetUniformLocation(primaryShaderProgram, "drag");
    glUniform1f(dragUniformLocation, drag);
	
	GLuint deltaTimeUniformLocation = glGetUniformLocation(primaryShaderProgram, "deltaTime");
	glUniform1f(deltaTimeUniformLocation, deltaTime);

    GLuint forceMinumUniformLocation = glGetUniformLocation(primaryShaderProgram, "forceMinimum");
    forceMinimum = getGravitationalForce_32(mass, mass, distanceMaximum);
    glUniform1f(forceMinumUniformLocation, forceMinimum);

    GLuint massUniformLocation = glGetUniformLocation(primaryShaderProgram, "mass");
    glUniform1f(massUniformLocation, mass);
    
    GLuint speedCapUniformLocation = glGetUniformLocation(primaryShaderProgram, "speedCap");
    glUniform1f(speedCapUniformLocation, speedCap);
    
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
        if (glfwGetKey(primaryWindow, GLFW_KEY_I)) camOrbitingSpeed *= 1.001f;
        if (glfwGetKey(primaryWindow, GLFW_KEY_O)) camOrbitingSpeed /= 1.001f;

        // Orbit camera
        if (glfwGetKey(primaryWindow, GLFW_KEY_A)) camCurrentOrbitAngle -= camOrbitingSpeed;
        if (glfwGetKey(primaryWindow, GLFW_KEY_D)) camCurrentOrbitAngle += camOrbitingSpeed;

        // Particle mass
        if (glfwGetKey(primaryWindow, GLFW_KEY_X))
        {
            mass = min(FLT_MAX, mass * 1.01f);
            distanceMaximum = sqrtf((GRAVITATIONAL_CONSTANT_FLOAT * mass) / accelerationMinimum); // a = F / m;  F = G * (m1 * m2) / (d^2);  a = (G * m^2 / d^2) / m  <=>  a = G * m / d^2;  Solve for d:  <=>  a * d^2 = G * m  <=>  d^2 = (G * m) / a 
            glUniform1f(distanceMaximumUniformLocation, distanceMaximum);                         // <=>  d = sqrt( (G * m) / a )      Note: m1 = m2 here since all objects have the same mass, therefore m1 * m2 = m1^2 or m^2 for short 
            forceMinimum = getGravitationalForce_32(mass, mass, distanceMaximum);
            glUniform1f(forceMinumUniformLocation, forceMinimum);
        }
        if (glfwGetKey(primaryWindow, GLFW_KEY_C))
        {
            mass = max(FLT_MIN, mass / 1.01f);
            distanceMaximum = sqrtf((GRAVITATIONAL_CONSTANT_FLOAT * mass) / accelerationMinimum); // a = F / m;  F = G * (m1 * m2) / (d^2);  a = (G * m^2 / d^2) / m  <=>  a = G * m / d^2;  Solve for d:  <=>  a * d^2 = G * m  <=>  d^2 = (G * m) / a 
            glUniform1f(distanceMaximumUniformLocation, distanceMaximum);                         // <=>  d = sqrt( (G * m) / a )      Note: m1 = m2 here since all objects have the same mass, therefore m1 * m2 = m1^2 or m^2 for short
            forceMinimum = getGravitationalForce_32(mass, mass, distanceMaximum);
            glUniform1f(forceMinumUniformLocation, forceMinimum);
        }

        // Drag
        if (glfwGetKey(primaryWindow, GLFW_KEY_F)) drag = min(FLT_MAX, drag * 1.000'002f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_G)) drag = max(1.0f, drag / 1.000'002f);

        // Velocity cap
        if (glfwGetKey(primaryWindow, GLFW_KEY_1)) speedCap = min(FLT_MAX, speedCap * 1.002f);
        if (glfwGetKey(primaryWindow, GLFW_KEY_2)) speedCap = max(FLT_MIN, speedCap / 1.002f);

        // Move camera
        mat4 viewMatrix = { 0 };
        camX = sin(camCurrentOrbitAngle) * radius;
        camZ = cos(camCurrentOrbitAngle) * radius;
        glm_lookat(
            (vec3) { camX, camY, camZ },
            (vec3) { 0.0f, 0.0f, 0.0f },
            (vec3) { 0.0f, 1.0f, 0.0f },
            viewMatrix
        );
        // Send updated uniforms to GPU
        glUniformMatrix4fv(viewMatrixUniformLocation, 1, GL_FALSE, (GLfloat*)viewMatrix);
        glUniform1f(dragUniformLocation, drag);
        timeWarpedDeltaTime = timeWarp * deltaTime; //? Left off debugging delta time issue and weird center point, also left of finding out how to use breakpoints with my new cmake build, because vs is not playing along anymore
        glUniform1f(deltaTimeUniformLocation, timeWarpedDeltaTime);
		glUniform1f(massUniformLocation, mass);
		glUniform1f(speedCapUniformLocation, speedCap);

		parametersForCglmPerspective = glfwGetWindowUserPointer(primaryWindow);
		if (parametersForCglmPerspective->matrixGotChanged)
		{
			glUniformMatrix4fv(perspectiveProjectionMatrixUniformLocation, 1, GL_FALSE, (GLfloat*)perspectiveProjectionMatrix);
			parametersForCglmPerspective->matrixGotChanged = false;
		}

        // Main draw call
        glDrawArrays(GL_POINTS, 0, amountStars); //TODO find way to make this not need a VBO and make it just draw amountStars GL_POINTS in a way that doesn't waste RAM with an unused VBO, since all the is in the SSBO anyways!

        glfwSwapBuffers(primaryWindow);
        glfwPollEvents();

        // Output
        if (secondsToWaitForInfoOutputUpdate <= secondWaitedForInfoOutPutUpdate) // Limit the amount of times output get's printed
        {
            frameRate = 1 / (glfwGetTime() - timeLastFrame);
            printf_s("\rFPS: %-4.0lf Delta time: %-4.5f Time warp: %-6.1f Particle mass: %-7.2g  Drag: %-10f  Speed cap: %-12.1f  Camera speed: %-8.1f", frameRate, deltaTime, timeWarp, mass, drag, speedCap, cameraUserInputSpeed);
            secondWaitedForInfoOutPutUpdate = 0;
        }
        else
        {
            secondWaitedForInfoOutPutUpdate += glfwGetTime() - timeLastFrame;
        }
		deltaTime = glfwGetTime() - timeLastFrame;
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
//TODO add adaptive physics calculation capping. Specifically make accelerationMinimum adaptive

// Stuff to do at school
//TODO in cmake make sure target_sources also get ALL non std header files

/*
GOALS
    - Make it multi-threaded
    - Make it use vulkan
    - Make it optimized
    - Make it really pretty
    - Make it cluster computing - Maybe not might be inefficient because of avg network bandwidth
        - Make it have a render mode, that creates a video file - THE ONLY WAY CLUSTER COMPUTING WOULD MAKE SENSE IS WITH A RENDER MODE, BECAUSE THEN SLOW LAN TRANSMISSION SPEEDS WOULD NOT MATTER
        - Make a "render mode" where it records the positions of each of the vertices and everything so it is a bit like a recording but it will still allow you to move around in 3D
    - Make super clear docs so anyone can read code
    - Make YouTube video?
*/
//TODO clean up entire codebase
//TODO Make this project compile for MSVC, clang and GCC. Currently it does not work with MSVC
//* Space is so cool