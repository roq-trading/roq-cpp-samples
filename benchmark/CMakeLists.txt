set(TARGET_NAME "${PROJECT_NAME}-benchmark")

add_executable("${TARGET_NAME}"
  main.cpp
)

target_include_directories("${TARGET_NAME}" PUBLIC
  ${ROQ_API_INCLUDE_DIRS}
  ${ROQ_CLIENT_INCLUDE_DIRS}
)

target_link_libraries("${TARGET_NAME}"
  ${ROQ_CLIENT_LIBRARIES}
  benchmark::benchmark
)
