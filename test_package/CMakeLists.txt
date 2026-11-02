cmake_minimum_required(VERSION 4.5 FATAL_ERROR)

set(CMAKE_CXX_MODULE_STD ON)

project(PeriapsisConsumer LANGUAGES CXX)

include(CTest)
find_package(periapsis CONFIG REQUIRED)

add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE synodic::periapsis)

add_test(NAME consumer COMMAND consumer)
