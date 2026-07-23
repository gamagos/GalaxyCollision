# (C) Sebastian Fiault 2026

cmake_minimum_required (VERSION 4.2.3)
message("++ [WARNING] This project must be built for x86_64 for debugging with GPU debugging software to work!") #TODO make my custom messages start with "++ " so they fit better with standard cmake output while still being distinguishable
message("++ [INFO] Using CMake Version: ${CMAKE_VERSION}")
project(GalaxyCollision
	HOMEPAGE_URL "https://github.com/gamagos/GalaxyCollision" 
	VERSION 0.4
	DESCRIPTION "A physics simulation of two galaxies colliding"
	LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
#
# Config of specific options depending on OS, architectures and compilers
#
# +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

# /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#
# IMPORTANT: Release builds should always build static libraries and use static linking!
#            Debug builds should use however use dynamic linking to support the use of address sanitizers!
#
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#* Options for GCC and Clang
if (CMAKE_C_COMPILER_ID MATCHES "GCC|Clang")
	# ===============================
	# Common options
	# ===============================
	add_compile_options(-Wall)
	add_link_options(-Wall)

	# ===============================
	# Options for Debug mode
	# ===============================
	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		message("++ [INFO] BUILDING DEBUG MODE") #TODO make variables for tags like [INFO] [WARNING] etc. in CMake
		# --------------------------------------------------------
		# Address sanitization for easier debugging of memory bugs
		# --------------------------------------------------------
		add_compile_options(-g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address)
		add_link_options(-g -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address)
	endif()

	# ===============================
	# Options for Release mode
	# ===============================
	if (CMAKE_BUILD_TYPE MATCHES "Release")
		message("++ [INFO] BUILDING Release MODE") #TODO make variables for tags like [INFO] [WARNING] etc. in CMake
		add_compile_options(-O3 -static)
		add_link_options(-O3 -static)
	endif()
# Options for MSVC
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
	# ===============================
	# Options for Debug mode
	# ===============================
	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded") #TODO add more debug and warning flags if building for debug
	endif()

	# ===============================
	# Options for Release mode
	# ===============================
	if (CMAKE_BUILD_TYPE MATCHES "Release")
	endif()
else()
	message("++ [WARNING] Unknown compiler. Flags could not be applied, this may cause errors") #TODO make variable for tags
endif()

add_subdirectory("Main")

# Make sure shader files are copied into build dir on every build
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Main/Resources")
set(RESOURCE_DIR_IN_BUILD_DIR "${CMAKE_BINARY_DIR}/Main/Resources")
add_custom_target(shaders
	COMMAND
		"${CMAKE_COMMAND}" -E copy_directory "${RESOURCE_DIR}" "${RESOURCE_DIR_IN_BUILD_DIR}"
)
add_dependencies(GalaxyCollision shaders)
enable_testing()

#TODO: Add tests
