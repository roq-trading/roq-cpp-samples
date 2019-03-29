cmake_minimum_required(VERSION 3.1)

project(roq-samples2)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

if(DEFINED ENV{CONDA_PREFIX})
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

project(${PACKAGE_NAME} CXX)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

find_package(Threads REQUIRED)
find_package(Gflags REQUIRED)
find_package(RoqApi REQUIRED)
find_package(RoqClient REQUIRED)

option(BUILD_BENCHMARK "Enable benchmark" OFF)

if(BUILD_BENCHMARK)
  find_package(Benchmark REQUIRED)
  add_subdirectory(benchmark)
endif(BUILD_BENCHMARK)

add_subdirectory(src)

option(BUILD_TEST "Enable test" ON)

if(BUILD_TEST)
  add_subdirectory(third_party/googletest/googletest)
  enable_testing()
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  add_subdirectory(test)
endif(BUILD_TEST)
