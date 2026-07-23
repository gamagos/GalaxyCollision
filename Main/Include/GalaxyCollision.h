// (C) Sebastian Fiault
//! +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//!
//! This file serves as a default header file for all files related to this project and should thus be included by all project files!
//!
//! +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__linux__)
	#include <linux/limits.h>
#endif

#ifndef GALAXY_COLLISION_H
#define GALAXY_COLLISION_H

	#ifndef NULL
	#define NULL ((void*)0i)
	#endif

	// Source - https://stackoverflow.com/a/3437433
	// Posted by dan04
	// Retrieved 2026-07-18, License - CC BY-SA 2.5
	// --------------------------------------------
	//! Remember this is a macro and not a function and thus behaves differently!
	//! It causes bugs with var++ and other things so look at the macro definition if you are unsure!
	#ifndef max
		#define max(x, y) (((x) > (y)) ? (x) : (y))
	#endif
	#ifndef min
		#define min(x, y) (((x) < (y)) ? (x) : (y))
	#endif
	// All caps version
	#ifndef MAX
		#define MAX(x, y) (((x) > (y)) ? (x) : (y))
	#endif
	#ifndef MIN
		#define MIN(x, y) (((x) < (y)) ? (x) : (y))
	#endif

	typedef int errno_t; // Because some implementations are dumb and do not define this

	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
	// 
	// Macros enabling cross platform support
	// 
	// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
	#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
		#ifndef gamagos_OS_IS_WINDOWS
		#define gamagos_OS_IS_WINDOWS true
		#endif
	#endif

	
	#if defined(__linux__)
		#ifndef _FULLPATH
			#define _FULLPATH
			/*
			SYNOPSIS:
				Definition of _fullpath if not defined by standard libraries.
				Usage is identical to the standard _fullpath
			RETURNS:
				The absolute path of the relative path in a malloc allocated buffer. Do not forget to free the buffer after use!
			REMARKS:
				Do not forget to free the resulting buffer that contains the full path after you do not need it anymore!!
			*/
			char* _fullpath(char* absolutePath, const char* relativePath, size_t maxSize);
		#endif

		#ifndef PRINTF_S
			#define PRINTF_S
			/* 
			SYNOPSIS:
				Definition of printf_s if not defined by standard libraries.
				Usage is identical to printf()
			*/
			void printf_s(const char* _format, ...); 
		#endif
		
		#ifndef _MAX_PATH
			//! [WARNING] To be able to use this macro correctly on linux systems you have to include <linux/limits.h> in the file where you want to use it!
			#define _MAX_PATH PATH_MAX
		#endif
	#endif

#endif
