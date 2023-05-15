target_sources(synodic-template-library
  PUBLIC
    FILE_SET structures TYPE CXX_MODULES FILES
	"fixed_vector.ixx"
    "queue.ixx"
	"graph.ixx"
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)

