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

Useful numbers:
    Age of universe: ~14 * 10^6 years = ~4.41806 * 10^17 Seconds (s)
    Mass of largest observed black hole: ~1 * 10^11 Solar masses (SU) = 1.989 * 10^14 Quettagrams
    Size of Milky-way: ~9 Kiloparsec (kpc) =
    See constants.h for more
    Time to Andromeda galaxy collision = ~4.5 * 10^6 years = 1.42 * 10^17 Seconds (s)

Useful constants:
    Minimal star mass: ~0.06 Solar masses = 119.34 Quettagrams (Qq)
    See constants.h for more

Useful SI prefixes:
    Quetta (Q): 10^30
    Ronna (R): 10^27
    Zetta (Z): 10^21
    Exa (E): 10^18
    Peta (P): 10^15
    Terra (T): 10^12
    Giga (G): 10^9

Useful units:
    1 Astronomical Unit (AU) = 1.495979 * 10^11 Meters (m) = 149.5979 Gigameters (Gm)
    1 Lightyear (ly) = 9.4607 * 10^15 Meters (m) = 9.4607 Petameters (Pm)
    1 Parsec (pc) = 3.0857 * 10^16 Meters (m) = 30.857 Petameters (Pm)
    1 Sun mass = 1.989 * 10^30 Kilograms (kg) = 1.989 * 10^3 Quettagrams = 1.989 Ronnatons (Rt)
    1 Ton (t) = 1000 Kilograms (kg)
    1 Year = 3.15506 * 10^7 Seconds (s)

Units used:
    Distances: Petameters (Pm)
    Time: Petaseconds (Ps)
    Velocity: Kilometers per Second (km/s)
    Weight: 10^6 Quettagrams (Qg)
*/

#include <process.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#include "vulkan/vulkan.h" // for backend simulation compute

#include "Types.h"

double timeSinceStartSeconds = 0.0;

int main(int argc, char **argv) 
{
    printf("Starting program \n");
    
    // Vulkan version
    uint32_t version;
    vkEnumerateInstanceVersion(&version);
    printf("Using Vulkan version %d.%d.%d \n", VK_VERSION_MAJOR(version), VK_VERSION_MINOR(version), VK_VERSION_PATCH(version));

    printf("Compiled with C Version: %ld \n", __STDC_VERSION__);
    printf("System integer size is %llu bytes (%llu bits) \n", sizeof(int), sizeof(int) * 8);
    
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
    - Make yt video?
*/

//* Space is so cool