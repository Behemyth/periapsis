cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}) #TODO: remove with new Conan generator
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}) #TODO: remove with new Conan generator

#Global Options
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON) # Opposite module/PCF order for `find_package`
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Group the default targets in a folder

project(SynodicTemplateLibrary 
	VERSION 0.1.0
	DESCRIPTION "Synodic template library"
	LANGUAGES CXX
)

option(BUILD_TESTS "Build test projects" ON)
option(BUILD_SHARED_LIBS "Build shared library" OFF)


add_library(${PROJECT_NAME})
add_library(synodic::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
PROPERTIES 
	LINKER_LANGUAGE CXX
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS NO
)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

target_include_directories(${PROJECT_NAME}
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_sources(${PROJECT_NAME}
PRIVATE
	${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
)

#Provides Visual Studio filter support
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES SOURCES)


if(BUILD_TESTS)

	enable_testing()
    add_subdirectory(tests)

endif()