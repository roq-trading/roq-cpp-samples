set(TARGET_NAME "${PROJECT_NAME}-republish-flags")

set(SOURCES flags.cpp)

add_library("${TARGET_NAME}" OBJECT ${SOURCES})

target_link_libraries("${TARGET_NAME}" PRIVATE roq-api::roq-api absl::flags)
