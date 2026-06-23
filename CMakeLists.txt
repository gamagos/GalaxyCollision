# (C) Sebastian Fiault 2026

cmake_minimum_required (VERSION 4.2.3)
message ("Using CMake Version: ${CMAKE_VERSION}")
project (GalaxyCollision VERSION 0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
if (CMAKE_C_COMPILER_ID MATCHES "GCC|Clang")
	set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")  # MSVC's multi threaded flag weirdly enough also makes things link statically, which we need

# Custom CMake variables for building this project
set(CGLM_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/cglm-0.9.6/include/")
set(GLAD_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/glad/include/")
set(GLFW_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/glfw-3.4/include/")
set(STBI_IMAGE_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Include/stb_image/")
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

# Add compiler flags
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang") #? Currently empty because they may make debugging harder
	add_compile_options("/Zi /fsanitize=address")
	add_link_options("/DEBUG")
elseif (CMAKE_C_COMPILER_ID MATCHES "MSVC")
	add_compile_options("-g -fsanitize=address -fno-omit-frame-pointer")
	add_link_options(-fsanitize=address)
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


enable_testing()

# TODO: Add tests and install targets if needed.
