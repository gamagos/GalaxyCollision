# (C) Sebastian Fiault 2026

cmake_minimum_required (VERSION 4.2.3)
message ("Using CMake Version: ${CMAKE_VERSION}")
project (GalaxyCollision VERSION 0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")  # MSVC's multithreaded flag weirdly enough also makes things link statically, which we need
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Custom CMake variables for building this project
set(CGLM_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/cglm-0.9.6/include/")
set(GLAD_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/glad/include/")
set(GLFW_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Libraries/glfw-3.4/include/")
set(STBI_IMAGE_INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/Main/Include/stb_image/")

# Add non build files to build dir
file(COPY "${CMAKE_SOURCE_DIR}/Main/Resources/" DESTINATION "${CMAKE_BINARY_DIR}/Main/Resources")

add_subdirectory("Main")
#add_subdirectory("Tests")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


enable_testing()

# TODO: Add tests and install targets if needed.
