cmake_minimum_required(VERSION 3.0)

project(common)

add_library(common STATIC
  account.cpp
  base_strategy.cpp
  config.cpp
  config_variables.cpp
  gateway.cpp
  instrument.cpp
  position.cpp
)

set_target_properties(common PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(common PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CCTZ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
)
