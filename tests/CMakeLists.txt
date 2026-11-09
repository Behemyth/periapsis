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

if(CMAKE_CONFIGURATION_TYPES)
	set(_test_is_multi_config ON)
else()
	set(_test_is_multi_config OFF)
endif()
add_test(
	NAME test_package_consumer
	COMMAND ${CMAKE_COMMAND}
		"-DTEST_PROJECT_BINARY_DIR=${CMAKE_BINARY_DIR}"
		"-DTEST_SOURCE_DIR=${CMAKE_SOURCE_DIR}/test_package"
		"-DTEST_GENERATOR=${CMAKE_GENERATOR}"
		"-DTEST_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}"
		"-DTEST_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}"
		"-DTEST_CONFIG=$<CONFIG>"
		"-DTEST_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
		"-DTEST_IS_MULTI_CONFIG=${_test_is_multi_config}"
		"-DTEST_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
		"-DTEST_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}"
		"-DTEST_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
		"-DTEST_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
		"-DTEST_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}"
		"-DTEST_CXX_STDLIB_MODULES_JSON=${CMAKE_CXX_STDLIB_MODULES_JSON}"
		-P "${CMAKE_CURRENT_LIST_DIR}/test_package.cmake"
)
