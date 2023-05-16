target_sources(synodic-template-library
	PUBLIC
		FILE_SET CXX_MODULES
		BASE_DIRS
			"${PROJECT_SOURCE_DIR}/library"
		FILES
			"type.ixx"
)

add_subdirectory(
	"concepts"
)

add_subdirectory(
	"traits"
)
