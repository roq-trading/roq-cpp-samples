cmake_minimum_required(VERSION 3.0)

project(benchmark)

add_executable(roq-samples-benchmark
  main.cpp
)

set_target_properties(roq-samples-benchmark PROPERTIES
  COMPILE_DEFINITIONS ROQ_SPDLOG
)

target_include_directories(roq-samples-benchmark PUBLIC
  ${ROQ_INCLUDE_DIR}
  ${CROSSGUID_INCLUDE_DIR}
  ${FLATBUFFERS_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${LIBUNWIND_INCLUDE_DIR}
  ${SPDLOG_INCLUDE_DIR}
  ${UCL_INCLUDE_DIR}
  ${BENCHMARK_INCLUDE_DIR}
)

target_link_libraries(roq-samples-benchmark
  ${ROQ_LIBS}
  ${CMAKE_THREAD_LIBS_INIT}
  ${CROSSGUID_LIBS}
  ${FLATBUFFERS_LIBS}
  ${GFLAGS_LIBS}
  ${LIBEVENT_LIBS}
  ${LIBUNWIND_LIBS}
  ${SPDLOG_LIBS}
  ${UCL_LIBS}
  ${BENCHMARK_LIBS}
  common
  utilities
)
