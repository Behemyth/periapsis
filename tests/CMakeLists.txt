cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

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