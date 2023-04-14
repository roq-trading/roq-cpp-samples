set(TARGET_NAME "${PROJECT_NAME}-example-3-flags")

set(SOURCES flags.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" PRIVATE roq-api::roq-api absl::flags)
