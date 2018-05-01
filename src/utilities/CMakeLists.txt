cmake_minimum_required(VERSION 3.0)

project(utilities)

add_library(utilities STATIC
  csv_reader.cpp
  generator.cpp
)

set_target_properties(utilities PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(utilities PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CCTZ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
)
