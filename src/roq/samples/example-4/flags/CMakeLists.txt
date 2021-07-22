set(TARGET_NAME "${PROJECT_NAME}-example-4-flags")

set(SOURCES flags.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" absl::flags)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_17)
