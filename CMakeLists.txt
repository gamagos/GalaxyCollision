# (C) Sebastian Fiault 2026

cmake_minimum_required (VERSION 4.2.3)
message("[WARNING]  This project must be built for x86_64 for debugging with GPU debugging software to work!")
message ("Using CMake Version: ${CMAKE_VERSION}")
project(GalaxyCollision
	HOMEPAGE_URL "https://github.com/gamagos/GalaxyCollision" 
	VERSION 0.3
	DESCRIPTION "A physics simulation of two galaxies colliding"
	LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set whole project to static compilation
if (CMAKE_C_COMPILER_ID MATCHES "GCC|Clang")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded") #TODO add more debug and warning flags if building for debug
else()
	message("[WARNING] Unknown compiler. Flags could not be applied, this may cause errors")
endif()

# Custom CMake variables for building this project
set(CGLM_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/cglm-0.9.6/include/")
set(GLAD_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/glad/include/")
set(GLFW_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/glfw-3.4/include/")
set(GALAXY_COLLISION_HEADER_FILE "${CMAKE_SOURCE_DIR}/Main/Source/GalaxyCollision.h")

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

# TODO: Add tests
