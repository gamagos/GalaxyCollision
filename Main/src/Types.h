// (C) Sebastian Fiault 2026
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

// This file consists of custom types(structs) useful for galactic simulation

// int32 Vector3
typedef struct Vector3_Int32
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vector3_Int32;

// int64 Vector3 
typedef struct Vector3_Int64
{
    int64_t x;
    int64_t y;
    int64_t z;
} Vector3_Int64;

// 32 bit float Vector3 
typedef struct Vector3_Float32
{
    float x;
    float y;
    float z;
} Vector3_Float32;

// 64 bit double Vector3 
typedef struct Vector3_Double64
{
    double x;
    double y;
    double z;
} Vector3_Double64;

/*
SYNOPSIS:
    RGB color with alpha value(RGBA)
DESCRIPTION:
    Each value and the alpha value are represented as an 8 bit
    unsigned integer.
*/
typedef struct RGBAColor {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
} RGBAColor;

/*
SYNOPSIS:
    A struct for physics computation of gas struct
DESCRIPTION:
    A struct that contains all the values used in the physics computations and
    also the values used in rendering
*/
typedef struct GasCloud_32
{
    // Physics values
    Vector3_Int32 position_Terameters;
    Vector3_Float32 velocity_KilometersPerSecond;
    float mass_10_BillionQuettagrams;
    float standardGravitationalParameter_TerametersCubedPerPetasecondSquared;

    // Render values
    float luminosity_Quettalumen;
    int32_t distanceCamera_Terameters;
    RGBAColor color;
} GasCloud_32;

/*
SYNOPSIS:
    A struct for physics computation of gas struct
DESCRIPTION:
    A struct that contains all the values used in the physics computations and
    also the values used in rendering
*/
typedef struct GasCloud_64
{
    // Physics values
    Vector3_Int64 position_Kilometers;
    Vector3_Double64 velocity_KilometersPerSecond;
    double mass_10_BillionQuettagrams;
    double standardGravitationalParameter_KilometersCubedPerPetasecondSquared;

    // Render values
    double luminosity_Quettalumen;
    int64_t distanceCamera_Kilometers;
    RGBAColor color;
} GasCloud_64;

/*
SYNOPSIS:
    Star object using a maximum of 32 bit precision for it's values
REMARKS:
    Beware that the entire struct is larger than 32 bits
*/
typedef struct Star_32
{
    // Values for physics
    Vector3_Int32 position_Terameters;
    Vector3_Float32 velocity_KilometersPerSecond;
    float mass_10_BillionQuettagrams;
    float standardGravitationalParameter_TerametersCubedPerPetasecondSquared;

    // Additional values for rendering
    float luminosity_Quettalumen;
    int32_t distanceCamera_Terameters;
    RGBAColor color;
} Star_32;

/*
SYNOPSIS:
    Star object using a maximum of 64 bit precision
REMARKS:
    Beware that the struct is larger than 64 bits
*/
typedef struct Star_64
{
    // Values for physics
    Vector3_Int64 position_Kilometers;
    Vector3_Double64 velocity_KilometersPerSecond;
    double mass_10_BillionQuettagrams;
    double standardGravitationalParameter_KilometersCubedPerPetasecondSquared;               

    // Additional values for rendering
    double luminosity_Quettalumen;
    int64_t distanceCamera_Kilometers;
    RGBAColor color;
} Star_64;

/*
SYNOPSIS:
    Struct for dark matter with 32 bit precision
REMARKS:
    Remember that dark matter cannot physically collide with things and is needed to hold galaxies together.
    This struct is larger than 32 bits!
*/
typedef struct DarkMatter_32
{
    Vector3_Int32 position_Terameters;
    Vector3_Float32 velocity_KilometersPerSecond;
    float mass_10_BillionQuettagrams;
    float standardGravitationalParameter_TerametersCubedPerPetasecondSquared;
} DarkMatter_32;

/*
SYNOPSIS:
    Struct for dark matter with 64 bit precision
REMARKS:
    Remember that dark matter cannot physically collide with things and is needed to hold galaxies together.
    This struct is larger than 64 bits!
*/
typedef struct DarkMatter_64
{
    Vector3_Int64 position_Kilometers;
    Vector3_Double64 velocity_KilometersPerSecond;
    double mass_10_BillionQuettagrams;
    double standardGravitationalParameter_KilometersCubedPerPetasecondSquared;
} DarkMatter_64;

/*
SYNOPSIS:
    Black hole struct using 32 bit precision
DESCRIPTION:
    Black hole struct with all values needed for simulation and generation
    Note that the rotation values only serve as orientation for star generation
REMARKS:
    Beware that the entire struct itself is actually larger than 32 bits!
 */
typedef struct BlackHole_32
{
    // Values for physics
    Vector3_Int32 position_Terameters;
    Vector3_Float32 velocity_KilometersPerSecond;
    float mass_10_BillionQuettagrams;
    float standardGravitationalParameter_TerametersCubedPerPetaecondSquared;

    // Additional values for galaxy generation
    Vector3_Float32 rotationAxis;
    bool rotatesClockwise;
} BlackHole_32;

/* 
SYNOPSIS:
    Black hole struct using 64 bit precision
DESCRIPTION:
    Black hole struct with all values needed for simulation and generation
    Note that the rotation values only serve as orientation for star generation
REMARKS:
    Beware that the struct itself is actually larger than 64 bits!
*/
typedef struct BlackHole_64
{
    // Values for physics
    Vector3_Int64 position_Kilometers;
    Vector3_Double64 velocity_KilometersPerSecond;
    double mass_10_BillionQuettagrams;
    double standardGravitationalParameter_KilometersCubedPerPetasecondSquared;
    
    // Additional values for galaxy collision
    Vector3_Double64 rotationAxis;
    bool rotatesClockwise;
} BlackHole_64;

#endif
