cmake_minimum_required(VERSION 3.0)

project(benchmark)

add_executable(roq-samples-benchmark
  main.cpp
)

target_include_directories(roq-samples-benchmark PUBLIC
  ${ROQ_API_INCLUDE_DIR}
  ${ROQ_CLIENT_INCLUDE_DIR}
)

target_link_libraries(roq-samples-benchmark
  ${CMAKE_THREAD_LIBS_INIT}
  ${ROQ_API_LIBS}
  ${ROQ_CLIENT_LIBS}
)
