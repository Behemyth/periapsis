
target_sources(synodic-template-library
  PUBLIC
    FILE_SET library TYPE CXX_MODULES FILES
	"library.ixx"
 "container/container.ixx")

add_subdirectory(
	"algorithm"
)

add_subdirectory(
	"container"
)


add_subdirectory(
	"exception"
)

add_subdirectory(
	"function"
)

add_subdirectory(
	"structure"
)

add_subdirectory(
	"string"
)

add_subdirectory(
	"thread"
)


add_subdirectory(
	"type"
)

add_subdirectory(
	"utility"
)
