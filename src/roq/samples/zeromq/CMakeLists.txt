set(TARGET_NAME "${PROJECT_NAME}-zeromq")

add_subdirectory(flags)
add_subdirectory(zmq)

add_executable("${TARGET_NAME}" application.cpp config.cpp strategy.cpp main.cpp)

target_link_libraries(
  "${TARGET_NAME}"
  PRIVATE ${TARGET_NAME}-flags
          ${TARGET_NAME}-zmq
          roq-client::roq-client
          roq-logging::roq-logging
          absl::flags
          fmt::fmt
          zmq)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)

target_compile_definitions("${TARGET_NAME}" PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})
