// (C) Sebastian Fiault 2026
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
void getPointsDistance_GenericFail()
{
	fprintf(stderr,
		"%s %s getPointsDistance only accepts (Vector3Int32, Vector3Int32) or (Vector3Int64, Vector3Int64)",
		FATAL_TAG,
		ERROR_TAG);
	exit(EXIT_FAILURE);
}