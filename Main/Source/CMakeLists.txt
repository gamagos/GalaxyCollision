# (C) Sebastian Fiault 2026

add_subdirectory(Utils)

add_executable(GalaxyCollision)

target_include_directories(GalaxyCollision
	PUBLIC
		${CGLM_INCLUDE_DIRECTORY}
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
		${STBI_IMAGE_INCLUDE_DIRECTORY}
)

target_sources(GalaxyCollision
	PRIVATE
		Main.c

	PUBLIC
		FILE_SET HEADERS
		TYPE HEADERS
		BASE_DIRS .
		FILES
			Constants.h
			Types.h

	PUBLIC
		${CGLM_INCLUDE_DIRECTORY}
		${GLAD_INCLUDE_DIRECTORY}
		${GLFW_INCLUDE_DIRECTORY}
		${STBI_IMAGE_INCLUDE_DIRECTORY}
)

target_link_libraries(GalaxyCollision
	PRIVATE
		DataUtils
		FileUtils
		GenerationUtils
		MathUtils
		OpenGLUtils
		PhysicsUtils
		
		cglm
		glad
		glfw
)