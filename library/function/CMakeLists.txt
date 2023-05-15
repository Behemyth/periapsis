target_sources(synodic-template-library
  PUBLIC
    FILE_SET functions TYPE CXX_MODULES FILES
	"executor.ixx"
	"function.ixx"
    "function_ref.ixx"
)