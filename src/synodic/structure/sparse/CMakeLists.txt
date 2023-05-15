target_sources(synodic-template-library
  PUBLIC
    FILE_SET sparse TYPE CXX_MODULES FILES
	"bitmap.cppm"
	"hash_map.cppm"
	"hash_set.cppm"
	"vector.cppm"
)