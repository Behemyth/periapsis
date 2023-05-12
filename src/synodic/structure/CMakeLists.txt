target_sources(synodic-template-library
PRIVATE
	"queue.h"
	"sparse/array.h"
	"sparse/bitmap.h"
	"sparse/hashmap.h"
	"sparse/hashset.h"
	"sparse/vector.h"
)

add_subdirectory(
	sparse
)

add_subdirectory(
	lockfree
)