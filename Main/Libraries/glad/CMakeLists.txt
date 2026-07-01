add_subdirectory("include")
add_subdirectory("src")

add_library(glad)

# Include headers of glad and glfw
target_include_directories(glad
	PUBLIC
		${GLAD_INCLUDE_DIRECTORY}
)

target_sources(glad
	PRIVATE
		"${CMAKE_SOURCE_DIR}/Main/Libraries/glad/src/glad.c"
	PUBLIC
		"${CMAKE_SOURCE_DIR}/Main/Libraries/glad/include/glad/glad.h"
)