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