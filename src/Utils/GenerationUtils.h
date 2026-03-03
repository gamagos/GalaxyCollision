// (C) Sebastian Fiault 2026
#ifndef GENERATION_UTILS_H
#define GENERATION_UTILS_H

#include <stdint.h>

#include "../Types.h"

StarFloat32* generateStarsFloat32(uint32_t amount, BlackHoleFloat32 parentBlackHole);
StarDouble64* generateStarsDouble64(uint32_t amount, BlackHoleDouble64 parentBlackHole);

#endif