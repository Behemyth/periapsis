target_sources(synodic-template-library
  PUBLIC
    FILE_SET structures TYPE CXX_MODULES FILES
	"fixed_vector.cppm"
    "queue.cppm"
	"graph.cppm"
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)

