cmake_minimum_required(VERSION 3.12)

project(roq-samples)

# conda

if(DEFINED ENV{CONDA_PREFIX} AND NOT DEFINED ENV{CONDA_BUILD})
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

# rpath

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# c++

option(TEST_CPP20 "Enable c++20" OFF)

if(TEST_CPP20)
set(CMAKE_CXX_STANDARD 20)
else(TEST_CPP20)
set(CMAKE_CXX_STANDARD 17)
endif(TEST_CPP20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# filesystem

include(GNUInstallDirs)

# dependencies

find_package(absl REQUIRED)
find_package(fmt REQUIRED)
find_package(roq-api REQUIRED)
find_package(roq-client REQUIRED)
find_package(roq-logging REQUIRED)

# include

include_directories("${CMAKE_SOURCE_DIR}/src")

# sub-projects

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-1")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-2")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-3")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/example-4")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples/import")

# test

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(BUILD_TESTING)
  include(GoogleTest)
  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest" EXCLUDE_FROM_ALL)
  add_subdirectory("${CMAKE_SOURCE_DIR}/test")
endif(BUILD_TESTING)

# doxygen

option(BUILD_DOCS "Enable doxygen" OFF)

if(BUILD_DOCS)
  find_package(Doxygen)
  add_subdirectory("${CMAKE_SOURCE_DIR}/doxygen")
endif(BUILD_DOCS)

# benchmark

option(BUILD_BENCHMARK "Enable benchmark" ON)

if(BUILD_BENCHMARK)
  find_package(benchmark REQUIRED)
  add_subdirectory("${CMAKE_SOURCE_DIR}/benchmark")
endif(BUILD_BENCHMARK)
