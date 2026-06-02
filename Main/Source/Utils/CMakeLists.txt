# (C) Sebastian Fiault 2026
add_library(DataUtils)
target_sources(DataUtils
	PRIVATE 
		DataUtils.c

	PUBLIC
		FILE_SET HEADERS
		TYPE HEADERS
		BASE_DIRS .
		FILES
			DataUtils.h
)

add_library(FileUtils)
target_sources(FileUtils
	PRIVATE
		FileUtils.c
	
	PUBLIC
		FileUtils.h
)

add_library(GenerationUtils)
target_sources(GenerationUtils
	PRIVATE 
		GenerationUtils.c

	PUBLIC
		FILE_SET HEADERS
		TYPE HEADERS
		BASE_DIRS .
		FILES
			GenerationUtils.h
)

add_library(MathUtils)
target_sources(MathUtils
	PRIVATE 
		MathUtils.c

	PUBLIC
		FILE_SET HEADERS
		TYPE HEADERS
		BASE_DIRS .
		FILES
			MathUtils.h
)

add_library(OpenGLUtils)
target_include_directories(OpenGLUtils
	PUBLIC
		${GLAD_INCLUDE_DIRECTORY} # OpenGLUtils depends on glad and glfw
		${GLFW_INCLUDE_DIRECTORY}
)
target_sources(OpenGLUtils
	PRIVATE
		"${CMAKE_SOURCE_DIR}/Main/Source/Utils/OpenGLUtils.c"
	PUBLIC
		"${CMAKE_SOURCE_DIR}/Main/Source/Utils/OpenGLUtils.h"
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
)

add_library(PhysicsUtils)
target_sources(PhysicsUtils
	PRIVATE 
		PhysicsUtils.c

	PUBLIC
		FILE_SET HEADERS
		TYPE HEADERS
		BASE_DIRS .
		FILES
			PhysicsUtils.h
)