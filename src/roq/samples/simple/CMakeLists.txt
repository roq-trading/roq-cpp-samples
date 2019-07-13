cmake_minimum_required(VERSION 3.0)

project(simple)

add_executable(roq-samples-simple
  config.cpp
  strategy.cpp
  main.cpp
)

target_include_directories(roq-samples-simple PUBLIC
  ${GFLAGS_INCLUDE_DIR}
  ${ROQ_API_INCLUDE_DIR}
  ${ROQ_CLIENT_INCLUDE_DIR}
)

target_link_libraries(roq-samples-simple
  ${CMAKE_THREAD_LIBS_INIT}
  ${GFLAGS_LIBS}
  ${ROQ_API_LIBS}
  ${ROQ_CLIENT_LIBS}
)
