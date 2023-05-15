cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# TODO: Remove with IDE/CMake support
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

project(synodic-template-library
	VERSION 0.1.0
	DESCRIPTION "Synodic Template Library"
	LANGUAGES CXX
)

# Enable CTest for the project
include(CTest)


# Provide a namespaced alias
add_library(synodic-template-library STATIC)
add_library(synodic::synodic-template-library ALIAS synodic-template-library)

set_target_properties(synodic-template-library
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
)

add_subdirectory(
	library
)

#Provides Visual Studio filter support
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES SOURCES)


if(BUILD_TESTING)

    add_subdirectory(tests)

endif()