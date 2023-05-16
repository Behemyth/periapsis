target_sources(synodic-template-library
  PUBLIC
    FILE_SET CXX_MODULES FILES
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

