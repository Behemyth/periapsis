target_sources(synodic-template-library
  PUBLIC
    FILE_SET "concept" TYPE CXX_MODULES FILES
	"concepts.ixx"
	"shared.ixx"
	"container.ixx"
	"sequence_container.ixx"
	"associative_container.ixx"
)