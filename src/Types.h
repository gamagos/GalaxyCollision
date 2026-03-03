// (C) Sebastian Fiault 2026
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>


// 32 bit precision float Vector3
typedef struct Vector3Float32
{
    float x;
    float y;
    float z;
} Vector3Float32;


// 64 bit precision double Vector3
typedef struct Vector3Double64
{
    double x;
    double y;
    double z;
} Vector3Double64;


// RGB color with alpha value
typedef struct RGBAColor {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t Alpha;
} RGBAColor;


/*
SYNOPSIS:
    Star object using a maximum of 32 bit precision for it's values
REMARKS:
    Beware that the entire struct is larger than 32 bits
*/
typedef struct StarFloat32
{
    Vector3Float32 position;
    Vector3Float32 velocityKiloMetersPerSecond;
    float gravity;

    // These values are only used to tell raylib what the star should look like
    float brightness;
    float distanceCamera;
    RGBAColor color;
} StarFloat32;


/*
SYNOPSIS:
    Star object using a maximum of 64 bit precision
REMARKS:
    Beware that the struct is larger than 64 bits
*/
typedef struct StarDouble64
{
    Vector3Double64 position;
    Vector3Double64 velocityKiloMetersPerSecond;
    double gravity;               

    // These values are only used to tell raylib how the star should look like
    double brightness;
    double distanceCamera;
    RGBAColor color;
} StarDouble64;


/*
SYNOPSIS:
    Black hole struct using 32 bit precision
REMARKS:
    Beware that the entire struct is larger than 32 bits!
 */
typedef struct BlackHoleFloat32
{
    Vector3Float32 position;
    Vector3Float32 velocityKiloMetersPerSecond;
    Vector3Float32 rotationAxis;
    bool rotatesClockwise;
    float gravity;
} BlackHoleFloat32;


/* 
SYNOPSIS:
    Black hole struct using 64 bit precision
DESCRIPTION:
    Black hole struct with all values needed for simulation and generation
    Note that the rotation values only serve as orientation for star generation
REMARKS:
    Beware that the struct is larger than 64 bits!
*/
typedef struct BlackHoleDouble64
{
    Vector3Double64 position;
    Vector3Double64 velocityKiloMetersPerSecond;
    Vector3Double64 rotationAxis;
    bool rotatesClockwise;
    double gravity;
} BlackHoleDouble64;

#endif
