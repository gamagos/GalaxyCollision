// (C) Sebastian Fiault
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Include/GalaxyCollision.h"
#include "../../Include/Utils/DataUtils.h"

#ifdef __linux__
	#include <linux/limits.h>
#endif

/*
SYNOPSIS:
	Gets the absolute path from a relative path
ARGS:
	relativePath:
		The relative path of the path you want to get the absolute path of
RETURNS:
	The absolute path of the relative path provided or NULL if an error occurs.
	[Warning] this buffer needs to be deallocated using free()!
REMARKS:
	Does not work with paths longer than "_MAX_PATH"!
	You have to free() the char array after use!
*/
char* getAbsolutePath(const char* relativePath);

/*
SYNOPSIS:
	Reads in a file as an array of chars
ARGS:
	path:
		The path of the file to read as a string
RETURNS:
	The contents of a file in an array of chars or NULL if something goes wrong
REMARKS:
	Does not work with UTF-8!
	Don't forget to free() the char array after having used it!
*/
char* readFileAsCharArray(char* path);
