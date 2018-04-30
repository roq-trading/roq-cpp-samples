cmake_minimum_required(VERSION 3.0.2)

# FIXME(thraneh): test and install targets

# project

project(roq-samples)

# extra modules, e.g. find

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

# c++11

project(${PACKAGE_NAME} CXX)
set(CMAKE_CXX_STANDARD 11)

# include source directories

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# dependencies

find_package(Threads REQUIRED)
find_package(Libunwind REQUIRED)
find_package(Crossguid REQUIRED)
find_package(Spdlog REQUIRED)
find_package(Gflags REQUIRED)
find_package(DoubleConversion REQUIRED)
find_package(Cctz REQUIRED)
find_package(Flatbuffers REQUIRED)
find_package(Libevent REQUIRED)
find_package(Ucl REQUIRED)
find_package(Roq REQUIRED)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(roq_samples_common STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/base_strategy.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/config_variables.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/gateway.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/instrument.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/common/position.cpp
)

set_target_properties(roq_samples_common PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(roq_samples_common PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CCTZ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
)

add_library(roq_samples_utilities STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/csv_reader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/generator.cpp
)

set_target_properties(roq_samples_utilities PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(roq_samples_utilities PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CCTZ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
)

add_executable(roq_samples_collector
  ${CMAKE_CURRENT_SOURCE_DIR}/src/collector/collector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/collector/main.cpp
)

set_target_properties(roq_samples_collector PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(roq_samples_collector PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CCTZ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${DOUBLE_CONVERSION_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
)

target_link_libraries(roq_samples_collector
  ${CMAKE_THREAD_LIBS_INIT}
  ${ROQ_LIBS}
  ${CCTZ_LIBS}
  ${CROSSGUID_LIBS}
  ${DOUBLE_CONVERSION_LIBS}
  ${FLATBUFFERS_LIBS}
  ${GFLAGS_LIBS}
  ${LIBEVENT_LIBS}
  ${LIBUNWIND_LIBS}
  ${SPDLOG_LIBS}
  roq_samples_utilities
)

add_executable(roq_samples_simple
  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple/config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple/config_reader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple/strategy.cpp
)

set_target_properties(roq_samples_simple PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(roq_samples_simple PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CCTZ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${DOUBLE_CONVERSION_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
  ${UCL_INCLUDE_DIR}
)

target_link_libraries(roq_samples_simple
  ${CMAKE_THREAD_LIBS_INIT}
  ${ROQ_LIBS}
  ${CCTZ_LIBS}
  ${CROSSGUID_LIBS}
  ${DOUBLE_CONVERSION_LIBS}
  ${FLATBUFFERS_LIBS}
  ${GFLAGS_LIBS}
  ${LIBEVENT_LIBS}
  ${LIBUNWIND_LIBS}
  ${SPDLOG_LIBS}
  ${UCL_LIBS}
  roq_samples_common
  roq_samples_utilities
)
