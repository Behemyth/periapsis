cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}) #TODO: remove with new Conan generator
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}) #TODO: remove with new Conan generator

#Global Options
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON) # Opposite module/PCF order for `find_package`
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Group the default targets in a folder

project(synodic-template-library 
	VERSION 0.1.0
	DESCRIPTION "Synodic template library"
	LANGUAGES CXX
)

include(CTest)


add_library(synodic-template-library)
add_library(synodic::synodic-template-library ALIAS synodic-template-library )

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

target_include_directories(synodic-template-library
INTERFACE
	src
)

add_subdirectory(
	src/synodic
)

#Provides Visual Studio filter support
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES SOURCES)


if(BUILD_TESTING)

	enable_testing()
    add_subdirectory(tests)

endif()