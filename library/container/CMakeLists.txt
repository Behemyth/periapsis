target_sources(synodic-template-library
	PUBLIC
		FILE_SET CXX_MODULES
		BASE_DIRS
			"${PROJECT_SOURCE_DIR}/library"
		FILES
			"fixed_vector.ixx"
			"queue.ixx"
			"container.ixx"
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)

