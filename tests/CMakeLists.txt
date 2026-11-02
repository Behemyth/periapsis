cmake_minimum_required(VERSION 4.5 FATAL_ERROR)

project(tests
	DESCRIPTION "Test project for Periapsis"
	LANGUAGES CXX
)

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
	synodic::periapsis
	GTest::gtest
	GTest::gtest_main
)

# Tests feed constant zero denominators into guarded divisions, which MSVC's optimizer reports as C4723
target_compile_options(tests
	PRIVATE
	$<$<CXX_COMPILER_ID:MSVC>:/wd4723>
)

add_subdirectory(
	unit
)

add_subdirectory(
	bench
)

gtest_discover_tests(tests)
