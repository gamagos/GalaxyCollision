# (C) Sebastian Fiault 2026

cmake_minimum_required (VERSION 4.2.3)
message ("Using CMake Version: ${CMAKE_VERSION}")
project (GalaxyCollision VERSION 0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory("Main")
#add_subdirectory("Tests")

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


enable_testing()

# TODO: Add tests and install targets if needed.
