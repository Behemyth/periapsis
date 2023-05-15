target_sources(synodic-template-library
  PUBLIC
    FILE_SET lockfree TYPE CXX_MODULES FILES
	"spsc_queue.cppm"
)