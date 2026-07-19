# (C) Sebastian Fiault 2026
# DataUtils
add_library(DataUtils)
target_sources(DataUtils
	PRIVATE 
		DataUtils.c

	PUBLIC
		../../Include/Utils/DataUtils.h
)

# DebugUtils
add_library(DebugUtils)
target_sources(DebugUtils
	PRIVATE 
		DebugUtils.c

	PUBLIC
		../../Include/Utils/DebugUtils.h
)
target_link_libraries(DebugUtils
	PRIVATE
		DataUtils
		glad
)

# FileUtils
add_library(FileUtils)
target_sources(FileUtils
	PRIVATE
		FileUtils.c
	
	PUBLIC
		../../Include/Utils/FileUtils.h
)
target_link_libraries(FileUtils
	PRIVATE
		DataUtils
)

# Generation Utils
add_library(GenerationUtils)
target_sources(GenerationUtils
	PRIVATE 
		GenerationUtils.c

	PUBLIC
		../../Include/Utils/GenerationUtils.h
)
target_link_libraries(GenerationUtils
	PRIVATE
		DataUtils
		PhysicsUtils
		MathUtils
)

# MathUtils
add_library(MathUtils)
target_sources(MathUtils
	PRIVATE 
		MathUtils.c

	PUBLIC
		../../Include/Utils/MathUtils.h
)
target_link_libraries(MathUtils
	PRIVATE
		DataUtils
)

# OpenGLUtils
add_library(OpenGLUtils)
target_sources(OpenGLUtils
	PRIVATE
		OpenGLUtils.c
	PUBLIC
		../../Include/Utils/OpenGLUtils.h
)
target_link_libraries(OpenGLUtils
	PRIVATE
		cglm
		glad
		glfw
		DataUtils
		FileUtils
)

# PhysicsUtils
add_library(PhysicsUtils)
target_sources(PhysicsUtils
	PRIVATE 
		PhysicsUtils.c

	PUBLIC
		../../Include/Utils/PhysicsUtils.h
)

# UserInteractionUtils
add_library(UserInteractionUtils)
target_sources(UserInteractionUtils
	PRIVATE 
		UserInteractionUtils.c

	PUBLIC
		../../Include/Utils/UserInteractionUtils.h
)