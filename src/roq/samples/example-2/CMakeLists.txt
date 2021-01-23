set(TARGET_NAME "${PROJECT_NAME}-example-2")

add_executable("${TARGET_NAME}" flags.cpp main.cpp)

target_link_libraries("${TARGET_NAME}" PRIVATE roq-client::roq-client roq-logging::roq-logging
                                               absl::flags fmt::fmt)

install(TARGETS ${TARGET_NAME})
