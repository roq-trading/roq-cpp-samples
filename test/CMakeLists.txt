cmake_minimum_required(VERSION 3.0)

project(test)

add_executable(roq-samples-test
  main.cpp
)

target_include_directories(roq-samples-test PUBLIC
  ${ROQ_API_INCLUDE_DIR}
  ${ROQ_CLIENT_INCLUDE_DIR}
)

target_link_libraries(roq-samples-test
  ${CMAKE_THREAD_LIBS_INIT}
  ${ROQ_API_LIBS}
  ${ROQ_CLIENT_LIBS}
  gtest_main
)

add_test (NAME roq-samples-test COMMAND roq-samples-test)
