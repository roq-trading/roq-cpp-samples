cmake_minimum_required(VERSION 3.0)

project(taker)

add_executable(roq-samples-taker
  config.cpp
  strategy.cpp
  main.cpp
)

target_include_directories(roq-samples-taker PUBLIC
  ${LIBUNWIND_INCLUDE_DIR}
  ${LIBEVENT_INCLUDE_DIR}
  ${GFLAGS_INCLUDE_DIR}
  ${ROQ_API_INCLUDE_DIR}
  ${ROQ_CLIENT_INCLUDE_DIR}
)

target_link_libraries(roq-samples-taker
  ${CMAKE_THREAD_LIBS_INIT}
  ${LIBUNWIND_LIBS}
  ${LIBEVENT_LIBS}
  ${GFLAGS_LIBS}
  ${ROQ_API_LIBS}
  ${ROQ_CLIENT_LIBS}
  common
)
