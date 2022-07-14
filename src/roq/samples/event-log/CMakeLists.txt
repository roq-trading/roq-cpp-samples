set(TARGET_NAME "${PROJECT_NAME}-event-log")

add_executable("${TARGET_NAME}" application.cpp processor.cpp main.cpp)

target_link_libraries(
  "${TARGET_NAME}"
  PUBLIC roq-api::roq-api roq-logging::roq-logging
  PRIVATE roq-client::roq-client absl::flags fmt::fmt)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)

target_compile_definitions("${TARGET_NAME}" PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})
