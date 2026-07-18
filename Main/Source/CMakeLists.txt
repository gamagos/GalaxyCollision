# (C) Sebastian Fiault 2026

add_subdirectory(Utils)

add_executable(GalaxyCollision)

target_sources(GalaxyCollision
	PRIVATE
		Main.c
		GalaxyCollision.c

	PUBLIC
		../Include/GalaxyCollision.h
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