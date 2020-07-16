set(TARGET_NAME "${PROJECT_NAME}-test")

add_executable("${TARGET_NAME}"
  main.cpp
)

target_link_libraries("${TARGET_NAME}"
  roq-client::roq-client
  gtest_main
)

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
