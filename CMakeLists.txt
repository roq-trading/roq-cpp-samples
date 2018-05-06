cmake_minimum_required(VERSION 3.0)

project(roq-samples)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

project(${PACKAGE_NAME} CXX)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

find_package(Threads REQUIRED)
find_package(Cctz REQUIRED)
find_package(Crossguid REQUIRED)
find_package(DoubleConversion REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(Gflags REQUIRED)
find_package(Libevent REQUIRED)
find_package(Libunwind REQUIRED)
find_package(Spdlog REQUIRED)
find_package(Ucl REQUIRED)
find_package(Roq REQUIRED)

add_subdirectory(src)
