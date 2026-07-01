# (C) Sebastian Fiault 2026

add_subdirectory(Utils)

add_executable(GalaxyCollision)

target_include_directories(GalaxyCollision
	PUBLIC
		${CGLM_INCLUDE_DIRECTORY}
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
)

target_sources(GalaxyCollision
	PRIVATE
		Main.c

	PUBLIC
		Constants.h
		GalaxyCollision.h
		${CGLM_INCLUDE_DIRECTORY}
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
		${GALAXY_COLLISION_HEADER_FILE}
)

target_link_libraries(GalaxyCollision
	PRIVATE
		DataUtils
		DebugUtils
		FileUtils
		GenerationUtils
		MathUtils
		OpenGLUtils
		PhysicsUtils
		UserInteractionUtils
		
		cglm
		glad
		glfw
)