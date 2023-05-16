target_sources(synodic-template-library
  PUBLIC
    FILE_SET algorithm.graph TYPE CXX_MODULES FILES
	"graph.ixx"
	"max_flow.ixx"
)