cmake_minimum_required(VERSION 3.0)

project(src)

add_subdirectory(common)
add_subdirectory(utilities)
add_subdirectory(collector)
add_subdirectory(simple)

if(BUILD_BENCHMARK)
add_subdirectory(benchmark)
endif(BUILD_BENCHMARK)
