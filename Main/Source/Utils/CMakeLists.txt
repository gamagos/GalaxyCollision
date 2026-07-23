# (C) Sebastian Fiault 2026
# DataUtils
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(DataUtils STATIC)
else()	
	add_library(DataUtils)
endif()
target_sources(DataUtils
	PRIVATE 
		DataUtils.c

	PUBLIC
		../../Include/Utils/DataUtils.h
)
target_link_libraries(DataUtils
	PRIVATE
		cglm
)

# DebugUtils
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(DebugUtils STATIC)
else()	
	add_library(DebugUtils)
endif()
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
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(FileUtils STATIC)
else()	
	add_library(FileUtils)
endif()
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
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(GenerationUtils STATIC)
else()	
	add_library(GenerationUtils)
endif()
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
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(MathUtils STATIC)
else()	
	add_library(MathUtils)
endif()
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
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(OpenGLUtils STATIC)
else()	
	add_library(OpenGLUtils)
endif()
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
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(PhysicsUtils STATIC)
else()	
	add_library(PhysicsUtils)
endif()
target_sources(PhysicsUtils
	PRIVATE 
		PhysicsUtils.c

	PUBLIC
		../../Include/Utils/PhysicsUtils.h
)

# UserInteractionUtils
if (CMAKE_BUILD_TYPE MATCHES "Release")
	add_library(UserInteractionUtils STATIC)
else()	
	add_library(UserInteractionUtils)
endif()
target_sources(UserInteractionUtils
	PRIVATE 
		UserInteractionUtils.c

	PUBLIC
		../../Include/Utils/UserInteractionUtils.h
)