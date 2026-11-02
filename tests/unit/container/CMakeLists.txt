target_sources(tests
	PRIVATE
	test_queue.cpp
	test_inplace_vector.cpp
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)
add_subdirectory(
	dynamic
)