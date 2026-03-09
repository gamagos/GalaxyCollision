// (C) Sebastian Fiault 2026
#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <stdint.h>

// This file consists of custom types(structs) useful for galactic simulation

// int32 Vector3
typedef struct Vector3Int32
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vector3Int32;

// int64 Vector3 
typedef struct Vector3Int64
{
    int32_t x;
    int32_t y;
    int32_t z;
} Vector3Int64;

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


// RGB color with alpha value (RGBA)
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
typedef struct Star32
{
    // Values for physics
    Vector3Int32 position;
    Vector3Float32 velocityKiloMetersPerSecond;
    float mass;
    float gravity;

    // Additional values for rendering
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
typedef struct Star64
{
    // Values for physics
    Vector3Int64 position;
    Vector3Double64 velocityKiloMetersPerSecond;
    double mass;
    double gravity;               

    // Additional values for rendering
    double brightness;
    double distanceCamera;
    RGBAColor color;
} StarDouble64;


/*
SYNOPSIS:
    Struct for dark matter with 32 bit precision
REMARKS:
    Remember that dark matter cannot physically collide with things and is needed to hold galaxies together.
    This struct is larger than 32 bits!
*/
typedef struct DarkMatter32
{
    Vector3Int32 position;
    Vector3Float32 velocity;
    float mass;
} DarkMatter32;

/*
SYNOPSIS:
    Struct for dark matter with 64 bit precision
REMARKS:
    Remember that dark matter cannot physically collide with things and is needed to hold galaxies together.
    This struct is larger than 64 bits!
*/
typedef struct DarkMatter64
{
    Vector3Int64 position;
    Vector3Double64 velocity;
    double mass;
} DarkMatter64;


/*
SYNOPSIS:
    Black hole struct using 32 bit precision
REMARKS:
    Beware that the entire struct itself is actually larger than 32 bits!
 */
typedef struct BlackHole32
{
    // Values for physics
    Vector3Int32 position;
    Vector3Float32 velocityKiloMetersPerSecond;
    float mass;
    float gravity;

    // Additional values for galaxy generation
    Vector3Float32 rotationAxis;
    bool rotatesClockwise;
} BlackHoleFloat32;


/* 
SYNOPSIS:
    Black hole struct using 64 bit precision
DESCRIPTION:
    Black hole struct with all values needed for simulation and generation
    Note that the rotation values only serve as orientation for star generation
REMARKS:
    Beware that the struct itself is actually larger than 64 bits!
*/
typedef struct BlackHole64
{
    // Values for physics
    Vector3Int64 position;
    Vector3Double64 velocityKiloMetersPerSecond;
    double mass;
    double gravity;
    
    // Additional values for galaxy collision
    Vector3Double64 rotationAxis;
    bool rotatesClockwise;
} BlackHoleDouble64;

#endif
