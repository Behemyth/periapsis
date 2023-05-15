target_sources(synodic-template-library
  PUBLIC
    FILE_SET sparse TYPE CXX_MODULES FILES
	"bitmap.ixx"
	"hash_map.ixx"
	"hash_set.ixx"
	"vector.ixx"
)