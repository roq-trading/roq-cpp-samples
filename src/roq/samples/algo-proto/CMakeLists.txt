set(TARGET_NAME ${PROJECT_NAME}-algo-proto)

add_subdirectory(flags)

set(SOURCES application.cpp bridge.cpp config.cpp route.cpp settings.cpp main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

add_dependencies(${TARGET_NAME} ${TARGET_NAME}-flags-autogen-headers)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE ${TARGET_NAME}-flags
          ${PROJECT_NAME}-algo-strategies
          ${PROJECT_NAME}-algo-framework
          roq-client::roq-client
          roq-client::roq-client-flags
          roq-cache::roq-cache
          roq-logging::roq-logging
          roq-logging::roq-logging-flags
          roq-flags::roq-flags
          roq-api::roq-api
          tomlplusplus::tomlplusplus
          unordered_dense::unordered_dense
          fmt::fmt)

target_compile_definitions(${TARGET_NAME} PRIVATE ROQ_PACKAGE_NAME="${TARGET_NAME}" ROQ_BUILD_VERSION="${CMAKE_PROJECT_VERSION}")

install(TARGETS ${TARGET_NAME})
