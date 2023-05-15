target_sources(synodic-template-library
PRIVATE
	executor.h
)

target_sources(synodic-template-library
  PUBLIC
    FILE_SET functions TYPE CXX_MODULES FILES
    "function_ref.ixx"
)