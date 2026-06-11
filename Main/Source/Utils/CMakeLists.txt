# (C) Sebastian Fiault 2026
add_library(DataUtils)
target_sources(DataUtils
	PRIVATE 
		DataUtils.c

	PUBLIC
		../Constants.h
		../GalaxyCollision.h
		DataUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)
	#? Left off making projection matrix aspect ratio get adjusted correctly on resize, for that I was making a struct in Types.h to easily pass in all the parameters
	#? Also I was intending to implement the same struct simplification for quitProgramOnError and I also was changing the camera rotation from being constant to being only when the corresponding keys are pressed

add_library(DebugUtils)
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

add_library(FileUtils)
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

add_library(GenerationUtils)
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

add_library(MathUtils)
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

add_library(OpenGLUtils)
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

add_library(PhysicsUtils)
target_sources(PhysicsUtils
	PRIVATE 
		PhysicsUtils.c

	PUBLIC
		PhysicsUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)

add_library(UserInteractionUtils)
target_sources(UserInteractionUtils
	PRIVATE 
		UserInteractionUtils.c

	PUBLIC
		UserInteractionUtils.h
		${GALAXY_COLLISION_HEADER_FILE}
)