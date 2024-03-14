cmake_minimum_required(VERSION 3.12)

# cmake

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/scripts/cmake ${CMAKE_MODULE_PATH})

# project

project(roq-samples)

# conda

if(DEFINED ENV{CONDA_PREFIX} AND NOT DEFINED ENV{CONDA_BUILD})
  set(CMAKE_INSTALL_PREFIX $ENV{CONDA_PREFIX})
  set(CMAKE_FIND_ROOT_PATH $ENV{CONDA_PREFIX})
endif()

# rpath

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# c++

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

# filesystem

include(GNUInstallDirs)

# dependencies

find_package(absl REQUIRED)
find_package(fmt REQUIRED)
find_package(pybind11 REQUIRED)
find_package(roq-api REQUIRED)
find_package(roq-cache REQUIRED)
find_package(roq-client REQUIRED)
find_package(roq-codec REQUIRED)
find_package(roq-flags REQUIRED)
find_package(roq-io REQUIRED)
find_package(roq-logging REQUIRED)
find_package(roq-market REQUIRED)
find_package(roq-utils REQUIRED)
find_package(roq-web REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(unordered_dense REQUIRED)

find_program(CLANG_FORMAT clang-format REQUIRED)

# test

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(BUILD_TESTING)
  find_package(Catch2 REQUIRED)
endif()

# include

include_directories(${CMAKE_SOURCE_DIR}/src)

# sub-projects

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/roq/algo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/roq/samples)
