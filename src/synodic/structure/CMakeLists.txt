target_sources(synodic-template-library
PRIVATE
	"queue.h"
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)