target_sources(synodic-template-library
PRIVATE
	queue.h
	fixed_vector.h
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)