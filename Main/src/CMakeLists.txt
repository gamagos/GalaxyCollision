# (C) Sebastian Fiault 2026

add_subdirectory(Utils)

add_executable(GalaxyCollision)
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
)
target_link_libraries(GalaxyCollision
	PRIVATE
		DataUtils
		GenerationUtils
		MathUtils
		PhysicsUtils
)