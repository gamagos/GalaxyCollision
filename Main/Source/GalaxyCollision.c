// (C) Sebastian Fiault
//! +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//!
//! This file serves as a default header file for all files related to this project and should thus be included by all project files!
//! As the header file can sometimes "define" methods (when a method that should be available is not on the target platform) they get
//! defined here. THIS FILE MUST BE LINKED WITH ALL PROJECT FILES IN SOURCE/
//!
//! +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../Include/GalaxyCollision.h"

#if defined(__linux__)
	#include <linux/limits.h>
#endif

char* _fullpath(char* absolutePath, const char* relativePath, size_t maxSize)
{
    char* result = 0;

    if (absolutePath)
    {
        result = absolutePath;
    }
    else
    {
        result = calloc(maxSize, sizeof(char));
    }

    result = realpath(relativePath, NULL);

    return result;
}

void printf_s(const char* _format, ...)
{
    va_list args;
    va_start(args, _format);
    vprintf(_format, args);
    va_end(args);
}