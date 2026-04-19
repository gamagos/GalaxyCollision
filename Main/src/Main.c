// (C) Sebastian Fiault 2026

/*
This is not a scientific program!
This program is for visual purposes.
This program uses metric units for the representation of data instead of
astrological units.
SI prefixes are applied to make data fit into 32 bit floating point variables.

PROJECT DEPENDECIES:
       Vulkan SKD
       C23 or newer
       WINDOWS LIBS:
           process.h
           windows.h
*/

#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//#include <vulkan/vulkan.h> // for backend simulation compute
#include <Windows.h>

#include "Types.h"

float timeSinceStart_PetaSeconds_Float = 0.0f;
double timeSinceStart_PetaSeconds_Double = 0.0;

int main(int argc, char **argv) 
{
    printf("Starting program \n");
    
    // Vulkan version
    //uint32_t version;
    //vkEnumerateInstanceVersion(&version);
    //printf("Using Vulkan version %d.%d.%d \n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

    printf("Compiled with C Version: %ld \n", __STDC_VERSION__);
    printf("System default integer size is %llu bytes  (%llu bits) \n", sizeof(int), sizeof(int) * 8);

    printf("Starting galaxy generation");
    

    printf("\n");
    return 0;
}

//TODO add Vulkan License
//TODO add GPLv3 License
//TODO make functions actually use units!

//! Note to self, make as many calculations as possible via vulkan and let raylib only handle
//! presentation so we don't have to deal with platform specific GUIs

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