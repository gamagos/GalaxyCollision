// (C) Sebastian Fiault 2026

/*
This is not a scientific program!
This program is for visual purposes.
This program uses metric units for the representation of data instead of
astrological units.
SI prefixes are applied to make data fit into 32 bit floating point variables.

PROJECT DEPENDECIES:
       raylib
       Vulkan
       WINDOWS LIBS:
           process.h
           windows.h


Useful data type information:
    Float32:
        Max positive float32 number: ~3.4 * 10^38
        Min positive float32 number: ~3.4 * 10^-45
        Remarks: Prefer to use floats as most GPUs are optimized for 32 bit floating point arithmetic
    Double64:
        Max positive number: ~1.8 * 10^308
        Min positive double64 number: ~2.2 * 10^-308
        Remarks: Double64 runs much slower on modern GPUs than float32!

Useful measurements:
    In constants.h

Useful constants:
    In constants.h

Useful SI prefixes:
    quetta: 10^30
    exa: 10^18

Units used:
    Velocity: Meters per Second (m/s)
    Time: Seconds
*/

#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "raylib.h" // for frontend rendering
//TODO #include "vulkan/vulkan.h" // for backend simulation compute

#include "Types.h"

double timeSinceStartSeconds = 0.0;

int main(int argc, char **argv) 
{
    printf("Starting program\n"); 
    printf("Using raylib version %s\n", RAYLIB_VERSION);
    uint32_t version;
    vkEnumerateInstanceVersion(&version);
    printf("Using Vulkan version %d.%d.%d", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));
    
    printf("\n\n\n");
    return 0;
}

//TODO add Vulkan License
//TODO add GPLv3 License

//! Note to self, make as many calculations as possible via vulkan and let raylib only handle
//! presentation so we don't have to deal with platform specific GUIs

//* Space is so cool