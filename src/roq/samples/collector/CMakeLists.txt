cmake_minimum_required(VERSION 3.0)

project(collector)

add_executable(roq-samples-collector
  collector.cpp
  main.cpp
)

target_include_directories(roq-samples-collector PUBLIC
  ${GFLAGS_INCLUDE_DIR}
  ${ROQ_API_INCLUDE_DIR}
  ${ROQ_CLIENT_INCLUDE_DIR}
)

target_link_libraries(roq-samples-collector
  ${CMAKE_THREAD_LIBS_INIT}
  ${GFLAGS_LIBS}
  ${ROQ_API_LIBS}
  ${ROQ_CLIENT_LIBS}
)
