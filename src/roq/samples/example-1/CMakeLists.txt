set(TARGET_NAME "${PROJECT_NAME}-example-1")

add_executable("${TARGET_NAME}"
  main.cpp
)

target_include_directories("${TARGET_NAME}" PUBLIC
  ${GFLAGS_INCLUDE_DIRS}
  ${FMT_INCLUDE_DIRS}
  ${ROQ_LOGGING_INCLUDE_DIRS}
  ${ROQ_API_INCLUDE_DIRS}
  ${ROQ_CLIENT_INCLUDE_DIRS}
)

target_link_libraries("${TARGET_NAME}"
  ${ROQ_CLIENT_LIBRARIES}
  ${ROQ_LOGGING_LIBRARIES}
  fmt::fmt
  gflags
)

install(TARGETS ${TARGET_NAME})
