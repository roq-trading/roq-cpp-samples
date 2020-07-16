set(TARGET_NAME "${PROJECT_NAME}-benchmark")

add_executable("${TARGET_NAME}"
  main.cpp
)

target_link_libraries("${TARGET_NAME}"
  roq-client::roq-client
  benchmark::benchmark
)
