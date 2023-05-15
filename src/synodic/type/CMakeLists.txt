target_sources(synodic-template-library
  PUBLIC
    FILE_SET "type" TYPE CXX_MODULES FILES
	"type.ixx"
)

add_subdirectory(
	"concept"
)

add_subdirectory(
	"trait"
)
