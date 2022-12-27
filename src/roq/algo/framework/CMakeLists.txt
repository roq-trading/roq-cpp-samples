set(TARGET_NAME "${PROJECT_NAME}-algo-framework")

set(SOURCES instrument.cpp state.cpp)

add_library("${TARGET_NAME}" STATIC ${SOURCES})

target_link_libraries("${TARGET_NAME}" PRIVATE roq-api::roq-api absl::flags absl::flat_hash_map
                                               absl::flat_hash_set)

target_compile_features("${TARGET_NAME}" PUBLIC cxx_std_20)
