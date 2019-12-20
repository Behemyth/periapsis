cmake_minimum_required(VERSION 3.15.2 FATAL_ERROR)

#Set CMake tools location
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}) #TODO: remove with new Conan generator
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}) #TODO: remove with new Conan generator

#Global Options
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON) # Opposite module/PCF order for `find_package`
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Group the default targets in a folder


##############################################
#Project 

project(SynodicTemplateLibrary 
	VERSION 0.0.1
	DESCRIPTION "Utility library."
	LANGUAGES CXX
)


##############################################
#Options

option(BUILD_TESTS "Build test projects" OFF)
option(BUILD_SHARED_LIBS "Build shared library" OFF)


##############################################
#Project Target

add_library(SynodicTemplateLibrary)
add_library(synodic::SynodicTemplateLibrary ALIAS SynodicTemplateLibrary)


##############################################
#Sources

#Search for all CMakeLists in the project
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS Source/*)
file(GLOB_RECURSE INCLUDES CONFIGURE_DEPENDS Include/*)

set(FILES ${SOURCES} ${INCLUDES})

set_target_properties(SynodicTemplateLibrary
	PROPERTIES 
		LINKER_LANGUAGE CXX
		CXX_EXTENSIONS OFF	
		CXX_STANDARD 20
		CXX_STANDARD_REQUIRED ON
)

target_include_directories(SynodicTemplateLibrary
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_sources(SynodicTemplateLibrary
	PRIVATE
		${FILES}
)

#Provides Visual Studio filter support
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${FILES})


##############################################
#Subprojects

if(BUILD_TESTS)

    add_subdirectory(Tests)

endif()