cmake_minimum_required(VERSION 3.21.2 FATAL_ERROR)

#Global Options
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Group the default targets in a folder

project(synodic-template-library 
	VERSION 0.1.0
	DESCRIPTION "Synodic Template Library"
	LANGUAGES CXX
)

include(CTest)


add_library(synodic-template-library INTERFACE)
add_library(synodic::synodic-template-library ALIAS synodic-template-library)


target_include_directories(synodic-template-library
INTERFACE
	src
)

add_subdirectory(
	src/synodic
)

#Provides Visual Studio filter support
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES SOURCES)


if(BUILD_TESTS)

	enable_testing()
    add_subdirectory(tests)

endif()