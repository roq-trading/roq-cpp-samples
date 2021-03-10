set(TARGET_NAME "${PROJECT_NAME}-import")

add_executable("${TARGET_NAME}" application.cpp base64.cpp flags.cpp processor.cpp main.cpp)

target_link_libraries("${TARGET_NAME}" PRIVATE roq-logging::roq-logging absl::flags fmt::fmt)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_14)

install(TARGETS ${TARGET_NAME})
