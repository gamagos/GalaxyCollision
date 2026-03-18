// (C) Sebastian Fiault
#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "../Constants.h"

/*
SYNOPSIS:
	Prints an error message and exits the program.
	Used in the getPointsDistance _Generic in MathUtils.h
DESCRIPTION:
	Instead of writing the same error message 5 times this function is used and
	since it does not really fit MathUtils.h it is defined in DebugUtils.h
EXAMPLE:
	getPointsDistance_GenericFail(); // Just print error message and exits the program
*/
void getPointsDistance_GenericFail();

#endif
