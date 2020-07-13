set(TARGET_NAME "${PROJECT_NAME}-test")

add_executable("${TARGET_NAME}"
  main.cpp
)

target_include_directories("${TARGET_NAME}" PUBLIC
  ${ROQ_API_INCLUDE_DIRS}
  ${ROQ_CLIENT_INCLUDE_DIRS}
)

target_link_libraries("${TARGET_NAME}"
  ${ROQ_CLIENT_LIBRARIES}
  gtest_main
)

add_test(NAME "${TARGET_NAME}" COMMAND "${TARGET_NAME}")
