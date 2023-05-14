target_sources(tests
PRIVATE
	test_fixed_vector.cpp
	test_queue.cpp
)

add_subdirectory(
	graph
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)