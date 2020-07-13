cmake_minimum_required(VERSION 3.12)

# cmake

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# project (note! no version)

project(roq-samples)

# conda

if(DEFINED ENV{CONDA_PREFIX})
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

# rpath

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# c++

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# dependencies

find_package(RoqApi REQUIRED)
find_package(RoqClient REQUIRED)
find_package(RoqLogging REQUIRED)
find_package(fmt REQUIRED)
find_package(gflags REQUIRED)

# TODO(thraneh): when cmake is used for conda packaging
# find_package(roq-api REQUIRED)
# find_package(roq-client REQUIRED)
# find_package(roq-logging REQUIRED)

# sub-projects

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-1")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-2")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-3")

# test

option(ROQ_SAMPLES_TEST "Enable test" OFF)

if(ROQ_SAMPLES_TEST)
  include(GoogleTest)
  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest" EXCLUDE_FROM_ALL)
  enable_testing()
  add_subdirectory("${CMAKE_SOURCE_DIR}/test")
endif(ROQ_SAMPLES_TEST)

# benchmark

option(ROQ_SAMPLES_BENCHMARK "Enable benchmark" OFF)

if(ROQ_SAMPLES_BENCHMARK)
  find_package(benchmark REQUIRED)
  add_subdirectory("${CMAKE_SOURCE_DIR}/benchmark")
endif(ROQ_SAMPLES_BENCHMARK)
