// (C) Sebastian Fiault
//! ### This file serves as a default header file for all files related to this project and should thus be included by all project files! ###

#ifndef GALAXY_COLLISION_H
#define GALAXY_COLLISION_H

#ifndef NULL
#define NULL ((void*)0i)
#endif

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
	#ifndef gamagos_OS_IS_WINDOWS
	#define gamagos_OS_IS_WINDOWS
	#endif
#endif

#endif
