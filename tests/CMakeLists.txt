cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

# TODO: Remove with IDE/CMake support
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)

project(tests
	DESCRIPTION "Test project for Synodic Template Library"
	LANGUAGES CXX
)

find_package(GTest REQUIRED)

add_executable(tests)

set_target_properties(tests
	PROPERTIES
		LINKER_LANGUAGE CXX
		CXX_STANDARD 23
		CXX_STANDARD_REQUIRED ON
		CXX_EXTENSIONS NO
)

target_link_libraries(tests
PRIVATE
	${CMAKE_PROJECT_NAME}
    GTest::gtest_main
)

add_subdirectory(
	"unit"
)

include(GoogleTest)
gtest_discover_tests(tests)