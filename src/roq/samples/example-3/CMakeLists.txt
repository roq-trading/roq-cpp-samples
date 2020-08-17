set(TARGET_NAME "${PROJECT_NAME}-example-3")

add_executable("${TARGET_NAME}" main.cpp)

target_link_libraries("${TARGET_NAME}" PRIVATE roq-client::roq-client roq-logging::roq-logging
                                               fmt::fmt gflags)

install(TARGETS ${TARGET_NAME})
