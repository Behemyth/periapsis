cmake_minimum_required(VERSION 4.0 FATAL_ERROR)

project(benchmarks
    DESCRIPTION "Benchmark project for Periapsis"
    LANGUAGES CXX
)

add_executable(benchmarks)

set_target_properties(benchmarks
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS NO
)

target_link_libraries(benchmarks
    PRIVATE
    synodic::periapsis
    benchmark::benchmark
    benchmark::benchmark_main
)

add_subdirectory(
    math
)

# Register as a single CTest with the "benchmark" label.
# Using generator expression for correct multi-config path resolution.
add_test(NAME benchmarks COMMAND $<TARGET_FILE:benchmarks>)
set_tests_properties(benchmarks PROPERTIES LABELS "benchmark")
