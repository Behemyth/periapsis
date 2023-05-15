target_sources(synodic-template-library
  PUBLIC
    FILE_SET functions TYPE CXX_MODULES FILES
	"executor.ixx"
	"functional.ixx"
    "function_ref.ixx"
)