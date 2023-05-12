target_sources(tests
PRIVATE
	test_queue.cpp
)

add_subdirectory(
	graph
)

add_subdirectory(
	sparse
)