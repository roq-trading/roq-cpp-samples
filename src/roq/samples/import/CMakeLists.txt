set(TARGET_NAME "${PROJECT_NAME}-import")

add_subdirectory(flags)

add_executable("${TARGET_NAME}" application.cpp base64.cpp processor.cpp main.cpp)

target_link_libraries("${TARGET_NAME}" PRIVATE ${TARGET_NAME}-flags roq-logging::roq-logging
                                               absl::flags fmt::fmt-header-only)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)

target_compile_definitions("${TARGET_NAME}" PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})
