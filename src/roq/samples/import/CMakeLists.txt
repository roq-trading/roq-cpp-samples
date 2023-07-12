set(TARGET_NAME ${PROJECT_NAME}-import)

add_subdirectory(flags)

set(SOURCES application.cpp base64.cpp processor.cpp settings.cpp main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

add_dependencies(${TARGET_NAME} ${TARGET_NAME}-flags-autogen-headers)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE ${TARGET_NAME}-flags
          roq-logging::roq-logging
          roq-logging::roq-logging-flags
          roq-flags::roq-flags
          roq-api::roq-api
          fmt::fmt
          absl::flags)

target_compile_definitions(${TARGET_NAME} PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})
