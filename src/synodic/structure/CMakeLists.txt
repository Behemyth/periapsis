target_sources(synodic-template-library
PRIVATE
	"queue.h"
	"sparse/array.h"
	"sparse/bitmap.h"
	"sparse/hash_map.h"
	"sparse/hash_set.h"
	"sparse/vector.h"
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)