set(TARGET_NAME "${PROJECT_NAME}-example-1")

add_executable("${TARGET_NAME}" application.cpp config.cpp flags.cpp strategy.cpp main.cpp)

target_link_libraries("${TARGET_NAME}" PRIVATE roq-client::roq-client roq-logging::roq-logging
                                               absl::flags fmt::fmt)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_14)

install(TARGETS ${TARGET_NAME})
