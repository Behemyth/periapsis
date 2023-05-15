target_sources(synodic-template-library
PRIVATE
	traits.h
)

target_sources(synodic-template-library
  PUBLIC
    FILE_SET concepts TYPE CXX_MODULES FILES
    "concepts.ixx"
)