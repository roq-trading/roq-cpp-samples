set(TARGET_NAME "${PROJECT_NAME}-example-1")

add_subdirectory(flags)

add_executable("${TARGET_NAME}" application.cpp config.cpp strategy.cpp main.cpp)

target_link_libraries(
  "${TARGET_NAME}" PRIVATE ${TARGET_NAME}-flags roq-client::roq-client roq-logging::roq-logging
                           roq-api::roq-api fmt::fmt absl::flags)

target_compile_definitions("${TARGET_NAME}" PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})
