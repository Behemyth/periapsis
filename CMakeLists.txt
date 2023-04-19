cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

#Global Options
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Group the default targets in a folder

project(synodic-template-library 
	VERSION 0.1.0
	DESCRIPTION "Synodic Template Library"
	LANGUAGES CXX
)

# Enable CTest for the project
include(CTest)

# Provide a namespaced alias
add_library(synodic-template-library INTERFACE)
add_library(synodic::synodic-template-library ALIAS synodic-template-library)

target_include_directories(synodic-template-library
INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory(
	src/synodic
)

#Provides Visual Studio filter support
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES SOURCES)


if(BUILD_TESTING)

    add_subdirectory(tests)

endif()