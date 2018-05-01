cmake_minimum_required(VERSION 3.0)

project(simple)

add_executable(example-simple
  config.cpp
  config_reader.cpp
  main.cpp
  strategy.cpp
)

set_target_properties(example-simple PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(example-simple PUBLIC
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

target_link_libraries(example-simple
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
  common
  utilities
)
