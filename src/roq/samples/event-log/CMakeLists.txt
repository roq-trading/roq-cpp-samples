set(TARGET_NAME ${PROJECT_NAME}-event-log)

add_executable(${TARGET_NAME} application.cpp processor.cpp main.cpp)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE roq-client::roq-client
          roq-client::roq-client-flags
          roq-logging::roq-logging
          roq-logging::roq-logging-flags
          roq-flags::roq-flags
          roq-api::roq-api
          fmt::fmt)

target_compile_definitions(${TARGET_NAME} PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})
