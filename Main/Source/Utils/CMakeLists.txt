# (C) Sebastian Fiault 2026
add_library(DataUtils STATIC)
target_sources(DataUtils
	PRIVATE 
		DataUtils.c

	PUBLIC
		../Constants.h
		../GalaxyCollision.h
		DataUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)

add_library(DebugUtils STATIC)
target_include_directories(DebugUtils
	PUBLIC
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
)
target_sources(DebugUtils
	PRIVATE 
		DebugUtils.c

	PUBLIC
	    ../Constants.h
		../GalaxyCollision.h
		DebugUtils.h
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
		${GALAXY_COLLISION_HEADER_FILE}
)
target_link_libraries(DebugUtils
	PRIVATE
		DataUtils
)

add_library(FileUtils STATIC)
target_sources(FileUtils
	PRIVATE
		FileUtils.c
	
	PUBLIC
		../GalaxyCollision.h
		FileUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)
target_link_libraries(FileUtils
	PRIVATE
		DataUtils
)

add_library(GenerationUtils STATIC)
target_sources(GenerationUtils
	PRIVATE 
		GenerationUtils.c

	PUBLIC
		GenerationUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)
target_link_libraries(GenerationUtils
	PRIVATE
		DataUtils
		PhysicsUtils
		MathUtils
)

add_library(MathUtils STATIC)
target_sources(MathUtils
	PRIVATE 
		MathUtils.c

	PUBLIC
		MathUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)
target_link_libraries(MathUtils
	PRIVATE
		DataUtils
)

add_library(OpenGLUtils STATIC)
target_include_directories(OpenGLUtils
	PUBLIC
		${GLAD_INCLUDE_DIRECTORY} # OpenGLUtils depends on glad and glfw
		${GLFW_INCLUDE_DIRECTORY}
)
target_sources(OpenGLUtils
	PRIVATE
		OpenGLUtils.c
	PUBLIC
		OpenGLUtils.h
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
		${GALAXY_COLLISION_HEADER_FILE}
)
target_link_libraries(OpenGLUtils
	PRIVATE
		cglm
		glad
		glfw
		DataUtils
		FileUtils
)

add_library(PhysicsUtils STATIC)
target_sources(PhysicsUtils
	PRIVATE 
		PhysicsUtils.c

	PUBLIC
		PhysicsUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)

add_library(UserInteractionUtils STATIC)
target_sources(UserInteractionUtils
	PRIVATE 
		UserInteractionUtils.c

	PUBLIC
		UserInteractionUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)